#include "ui/file_selection.hpp"

void ui::FileSelection::Initialize(const sf::Vector2f& view_size, const std::shared_ptr<sf::Font>& font) {
    
    default_text_.setFont(*font);
    default_text_.setCharacterSize(constants::kFileSelectionFontSize);

    background_.setSize(sf::Vector2f(view_size.x - view_size.x * 2 * margin_, view_size.y - view_size.y * 2 * margin_));
    background_.setPosition(sf::Vector2f(view_size.x * margin_, view_size.y * margin_));
    background_.setFillColor(constants::kFSbgColor);
    background_.setOutlineColor(constants::kFSoutlineColor);
    background_.setOutlineThickness(2.f);

    visible_lines_ = background_.getSize().y / line_height_;

    //scrollbar_track_.setOutlineThickness(0.f);
    //scrollbar_thumb_.setOutlineThickness(0.f);
    scrollbar_track_.setFillColor(constants::kScrollbarTrackColor);
    scrollbar_thumb_.setFillColor(constants::kScrollbarThumbColor);

    UpdateScrollbarGeometry();
}

int ui::FileSelection::ScanFiles() {
    texts_.clear();
    last_clicked_path_.clear();
    last_clicked_index_ = -1;

    if (constants::debug) { core::PrintTestMsg("Scanning files in ", saves_folder_); }
    try {
        if (!fs::exists(saves_folder_) || !fs::is_directory(saves_folder_)) {
            std::cerr << "Not a directory: " << saves_folder_ << "\n";
            return EXIT_FAILURE;
        }
        for (const auto& file : fs::directory_iterator(saves_folder_)) {
            if (file.is_regular_file()) {
                std::ifstream inFile(file.path().string());
                std::time_t timestamp = static_cast<std::time_t>(core::GetIntFromLine(inFile));
                std::istringstream tmp(std::to_string(static_cast<long long>(timestamp)));
                std::string time_str = core::DecodeTimeFromFile(tmp);

                std::string label = time_str + "   " + file.path().stem().string();
                sf::Text text = default_text_;
                text.setString(label);
                text.setOutlineColor(constants::kFSselectedTextOutline);
                texts_.emplace_back(file.path(), std::move(text), timestamp);

                if (constants::debug) {
                    core::PrintTestMsg(file.path().string(), " created at: ", time_str, "   ", timestamp);
                }
            }
        }
    } catch (const fs::filesystem_error& error) {
        std::cerr << "Filesystem error: " << error.what() << "\n";
        return EXIT_FAILURE;
    }
    
    // sort texts_ to reverse chronological order
    std::stable_sort(texts_.begin(), texts_.end(), [](auto const& x, auto const& y) { return std::get<2>(x) > std::get<2>(y); });

    // Clamp scroll and refresh scrollbar
    SetScroll(std::min(scroll_, MaxScroll()));
    scrollbar_dirty_ = true;

    return 0;
};

void ui::FileSelection::Update(const sf::RenderWindow& window, const sf::Vector2f& mousePos, const sf::Event& event, const bool isLoadClicked) {

    auto windowSize = window.getSize();

    const bool overflow = IsOverflow();
    const bool mouseOverBackground = background_.getGlobalBounds().contains(mousePos);

    // Mouse wheel scrolling when cursor is over the list area
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (mouseOverBackground && overflow) {
            int steps = (event.mouseWheelScroll.delta < 0) ? 1 : -1;
            SetScroll(scroll_ + steps);
        }
    }

    // Start drag or page on track click
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (overflow) {
            if (scrollbar_thumb_.getGlobalBounds().contains(mousePos)) {
                scrollbar_dragging_ = true;
                scrollbar_grab_offset_y_ = mousePos.y - scrollbar_thumb_.getPosition().y;
            } else if (scrollbar_track_.getGlobalBounds().contains(mousePos)) {
                // Click on track pages up/down
                float thumbTop = scrollbar_thumb_.getPosition().y;
                float thumbBottom = thumbTop + scrollbar_thumb_.getSize().y;
                if (mousePos.y < thumbTop) {
                    SetScroll(scroll_ - visible_lines_); // page up
                } else if (mousePos.y > thumbBottom) {
                    SetScroll(scroll_ + visible_lines_); // page down
                }
            }
        }
    }

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        scrollbar_dragging_ = false;
    }

    // Dragging the thumb moves scroll
    if (event.type == sf::Event::MouseMoved && scrollbar_dragging_ && overflow) {
        const float trackY = scrollbar_track_.getPosition().y;
        const float trackH = scrollbar_track_.getSize().y;
        float thumbH = scrollbar_thumb_.getSize().y;

        float maxThumbTravel = std::max(0.f, trackH - thumbH);
        float targetTop = std::clamp(mousePos.y - scrollbar_grab_offset_y_, trackY, trackY + maxThumbTravel);

        float t = (maxThumbTravel > 0.f) ? (targetTop - trackY) / maxThumbTravel : 0.f;
        int newScroll = (int)std::round(t * MaxScroll());
        SetScroll(newScroll);
    }

    // for deselection check
    bool clicked = false;
    bool textSelected = false;

    // Update text positions, check for selections and set text parameters
    const sf::Vector2f bg_pos = background_.getPosition();
    const int end = std::min((int)texts_.size(), scroll_ + visible_lines_);
    for (int i = scroll_; i < end; ++i) {
        float y = bg_pos.y + content_padding_ + (i - scroll_) * line_height_;
        float x = bg_pos.x + content_padding_;
        std::get<1>(texts_[i]).setPosition(x, y);
        if (!isLoadClicked &&
            event.type == sf::Event::MouseButtonReleased && 
            event.mouseButton.button == sf::Mouse::Left) {

            if (overflow && (scrollbar_thumb_.getGlobalBounds().contains(mousePos) ||
                                scrollbar_track_.getGlobalBounds().contains(mousePos))) {
                continue;
            }

            if (std::get<1>(texts_[i]).getGlobalBounds().contains(mousePos)) {
                last_clicked_path_ = std::get<0>(texts_[i]).string();
                last_clicked_index_ = i;
                textSelected = true;
            } else {
                clicked = true;
            }
        }
    }

    if (!textSelected && clicked) {
        last_clicked_path_.clear();
        last_clicked_index_ = -1;
    }

    if (scrollbar_dirty_) {
        UpdateScrollbarGeometry();
        scrollbar_dirty_ = false;
    }
}

void ui::FileSelection::UpdateOutsideEventLoop(const sf::Vector2f& window_size, const bool& resized) {
    // update background
    if (resized) {
        background_.setSize(sf::Vector2f(window_size.x - window_size.x * 2 * margin_, window_size.y - window_size.y * 2 * margin_));
        background_.setPosition(sf::Vector2f(window_size.x * margin_, window_size.y * margin_));
        visible_lines_ = background_.getSize().y / line_height_;
        SetScroll(std::min(scroll_, MaxScroll())); // clamp within range
        scrollbar_dirty_ = true;
    }
}

void ui::FileSelection::Reset() { 
    last_clicked_path_.clear();
    scroll_ = 0;
    last_clicked_index_ = -1;
    texts_.clear();
    scrollbar_dragging_ = false;
    UpdateScrollbarGeometry();
}

void ui::FileSelection::DrawTo(sf::RenderWindow& window) {
    window.draw(background_);

    // Compute content rect
    sf::Vector2f pos = background_.getPosition() + sf::Vector2f(content_padding_, content_padding_);
    sf::Vector2f size = background_.getSize() - sf::Vector2f(2.f * content_padding_, 2.f * content_padding_);

    // Save current view
    sf::View prev = window.getView();

    // Build a view that maps content coords (0..size) to the on-screen rectangle (viewport)
    sf::View listView;
    listView.setCenter(size.x * 0.5f, size.y * 0.5f);
    listView.setSize(size);

    // Viewport is normalized [0..1] of the window
    sf::Vector2u ws = window.getSize();
    sf::FloatRect vp(pos.x / ws.x, pos.y / ws.y, size.x / ws.x, size.y / ws.y);
    listView.setViewport(vp);

    window.setView(listView);

    // Draw texts with local positions (0..size)
    const int end = std::min((int)texts_.size(), scroll_ + visible_lines_);
    for (int i = scroll_; i < end; ++i) {
        sf::Text t = std::get<1>(texts_[i]);
        t.setPosition(0.f, (i - scroll_) * line_height_); // local to content area
        t.setFillColor(i == last_clicked_index_ ? constants::kFSselectedTextColor : constants::kFStextColor);
        t.setOutlineThickness(i == last_clicked_index_ ? constants::kFileSelectionFontSize * 0.05f : 0.f);
        window.draw(t);
    }

    // Restore view
    window.setView(prev);

    // Draw scrollbar if needed
    if (IsOverflow()) {
        if (scrollbar_dirty_) {
            UpdateScrollbarGeometry();
            scrollbar_dirty_ = false;
        }
        window.draw(scrollbar_track_);
        window.draw(scrollbar_thumb_);
    }
}