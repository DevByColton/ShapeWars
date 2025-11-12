#include "../Include/OptionsMenu.h"
#include "../../Content/Include/GaussianBlur.h"
#include "../../Content/Include/Sound.h"
#include "../../Core/Include/Logger.h"
#include "../../Input/Include/Input.h"
#include "../../Input/Include/MouseAndKeyboard.h"


OptionsMenu::OptionsMenu()
{
    optionsBackgroundTexture.setSmooth(true);
    title.setPosition({optionsBackgroundTexture.getSize().x / 2.f, 50.f});
    title.setOrigin(title.getLocalBounds().getCenter());

    // Set the default active menu option
    optionIndicator.left.setPosition(vsync.containerSprite.getTransform().transformPoint({-20.f, vsync.containerSprite.getOrigin().y}));
    optionIndicator.right.setPosition(vsync.containerSprite.getTransform().transformPoint({
        vsync.containerSprite.getLocalBounds().size.x,
        vsync.containerSprite.getOrigin().y
    }));

    // Set back text and buttons
    backText.setOrigin(backText.getLocalBounds().getCenter());
    backText.setStyle(sf::Text::Bold);
    backText.setPosition({
        optionsBackgroundTexture.getSize().x / 2.f - 20.f,
        optionsBackgroundTexture.getSize().y - 50.f
    });
    buttons.escape.setPosition({
        backText.getPosition().x + 75.f,
        optionsBackgroundTexture.getSize().y - 48.5f
    });
    buttons.xboxBButton.setPosition(buttons.escape.getPosition());
    buttons.dualsenseCircleButton.setPosition(buttons.escape.getPosition());
    buttons.activeButtons.push_back(&buttons.escape);
    buttons.activeButtons.push_back(&buttons.xboxBButton);
    buttons.activeButtons.push_back(&buttons.dualsenseCircleButton);
    buttons.activeButtons.shrink_to_fit();

    // Set button actions
    vsync.onToggle = [this]{ toggleVsync(false); };

    // Updating vsync origin manually, the alignment issue strikes again
    vsync.label.setOrigin({vsync.label.getOrigin().x, vsync.label.getOrigin().y - 8.f});

    musicVol.onPress = [this]{ startSliding(&musicVol); };
    musicVol.onRelease = [this] { endSliding(); };
    musicVol.onUpdate = [this] { Sound::instance().setMusicMasterVolume(musicVol.value); };

    sfxVol.onPress = [this]{ startSliding(&sfxVol); };
    sfxVol.onRelease = [this] { endSliding(); };
    sfxVol.onUpdate = [this] { Sound::instance().setSfxMasterVolume(sfxVol.value); };

    buttonsOverride.onToggle = [this]{ toggleButtonOverride(false); };

    loadOptions();
}


void OptionsMenu::loadOptions()
{
    // Open the options file
    std::fstream optionsFile {"Content\\Data\\Options.bin", std::ios::in | std::ios::binary};

    if (!optionsFile)
        return;

    // Read the options and close
    optionsFile.read(reinterpret_cast<char*>(&options), sizeof(Options));
    optionsFile.close();

    toggleVsync(true);
    musicVol.updateValue(options.musicVolume);
    sfxVol.updateValue(options.sfxVolume);
    toggleButtonOverride(true);
    buttonsOverrideOptions.setByActiveIndex(options.buttonsOverrideOption);
}


void OptionsMenu::saveOptions()
{
    // Open the options file
    std::fstream optionsFile {"Content\\Data\\Options.bin", std::ios::out | std::ios::binary};

    if (!optionsFile)
        return;

    // Sync the current options with the options object
    options.vsync = vsync.checked;
    options.musicVolume = musicVol.value;
    options.sfxVolume = sfxVol.value;
    options.buttonsOverride = buttonsOverride.checked;
    options.buttonsOverrideOption = static_cast<int>(buttonsOverrideOptions.activeOption->first);

    // Write the file and close
    optionsFile.write(reinterpret_cast<char*>(&options), sizeof(Options));
    optionsFile.close();
}


OptionsMenu::SwitchOption::SwitchOption(const std::string& label, const bool checked, const sf::Vector2f& position)
{
    containerTexture.setSmooth(true);
    containerSprite.setPosition(position);
    containerSprite.setOrigin(containerSprite.getLocalBounds().getCenter());
    this->label.setString(label);
    this->label.setOrigin({0.f, this->label.getLocalBounds().getCenter().y});
    this->label.setPosition({10.f, containerTexture.getTexture().getSize().y / 2.f});
    this->label.setFillColor(MUTED_TEXT_COLOR);
    this->label.setStyle(sf::Text::Bold);
    this->checked = checked;
    switchChecked.setPosition({containerTexture.getTexture().getSize().x - 85.f, containerTexture.getTexture().getSize().y / 2.f});
    switchChecked.setOrigin(switchChecked.getLocalBounds().getCenter());
    switchUnchecked.setPosition(switchChecked.getPosition());
    switchUnchecked.setOrigin(switchUnchecked.getLocalBounds().getCenter());
}


OptionsMenu::SliderOption::SliderOption(const std::string& label, const sf::Vector2f& position)
{
    containerTexture.setSmooth(true);
    containerSprite.setOrigin(containerSprite.getLocalBounds().getCenter());
    containerSprite.setPosition(position);
    this->label.setString(label);
    this->label.setOrigin({0.f, this->label.getLocalBounds().getCenter().y});
    this->label.setPosition({10.f, containerTexture.getTexture().getSize().y / 2.f});
    this->label.setFillColor(MUTED_TEXT_COLOR);
    this->label.setStyle(sf::Text::Bold);
    outline.setPosition({containerTexture.getTexture().getSize().x - 205.f, containerTexture.getTexture().getSize().y / 2.f});
    outline.setOrigin(outline.getLocalBounds().getCenter());
    fill.setPosition(outline.getPosition());
    fill.setOrigin(fill.getLocalBounds().getCenter());
    knob.setOrigin(knob.getLocalBounds().getCenter());
    setValuePositions();
}


OptionsMenu::ButtonsOverrideOptions::ButtonsOverrideOptions(const std::string& label, const sf::Vector2f& position)
{
    containerTexture.setSmooth(true);
    containerSprite.setOrigin(containerSprite.getLocalBounds().getCenter());
    containerSprite.setPosition(position);
    this->label.setString(label);
    this->label.setOrigin({0.f, this->label.getLocalBounds().getCenter().y});
    this->label.setPosition({10.f, containerTexture.getTexture().getSize().y / 2.f});
    this->label.setFillColor(MUTED_TEXT_COLOR);
    this->label.setStyle(sf::Text::Bold);

    dualsense.second.setFillColor(MUTED_TEXT_COLOR);
    dualsense.second.setStyle(sf::Text::Bold);
    dualsense.second.setPosition({containerTexture.getTexture().getSize().x - 165.f, containerTexture.getTexture().getSize().y / 2.f});
    dualsense.second.setOrigin(dualsense.second.getLocalBounds().getCenter());

    xbox.second.setFillColor(MUTED_TEXT_COLOR);
    xbox.second.setStyle(sf::Text::Bold);
    xbox.second.setPosition({dualsense.second.getPosition().x - 200.f, dualsense.second.getPosition().y});
    xbox.second.setOrigin({xbox.second.getLocalBounds().getCenter().x, keyboard.second.getLocalBounds().getCenter().y});
    // Note: For whatever reason the text "xbox" does not align on the y axis properly with the other text. So to get around
    //       this, I am using the y origin of "keyboard" so it aligns with the other text. It has something to do with the 'b'

    keyboard.second.setFillColor(MUTED_TEXT_COLOR);
    keyboard.second.setStyle(sf::Text::Bold);
    keyboard.second.setPosition({xbox.second.getPosition().x - 180.f, containerTexture.getTexture().getSize().y / 2.f});
    keyboard.second.setOrigin(keyboard.second.getLocalBounds().getCenter());
}


void OptionsMenu::SliderOption::updateValue(const float next)
{
    if (next < MIN || next > MAX)
        return;

    value = next;
    onUpdate();

    setValuePositions();
}


void OptionsMenu::SliderOption::setValuePositions()
{
    const int x = fill.getTexture().getSize().x;
    const int y = fill.getTexture().getSize().y;
    const float sizeX = value / MAX * x;

    fill.setTextureRect({{0, 0},{static_cast<int>(sizeX), y}});
    knob.setPosition(fill.getTransform().transformPoint({sizeX, y / 2.f}));
}


void OptionsMenu::SwitchOption::draw(sf::RenderTexture& renderTexture)
{
    containerTexture.clear(sf::Color::Transparent);
    containerTexture.draw(label);
    containerTexture.draw(checked ? switchChecked : switchUnchecked);
    containerTexture.display();
    renderTexture.draw(containerSprite);
}


void OptionsMenu::SliderOption::draw(sf::RenderTexture& renderTexture)
{
    containerTexture.clear(sf::Color::Transparent);
    containerTexture.draw(label);
    containerTexture.draw(outline);
    containerTexture.draw(fill);
    containerTexture.draw(knob);
    containerTexture.display();
    renderTexture.draw(containerSprite);
}


void OptionsMenu::ButtonsOverrideOptions::checkOptionsSelected()
{
    const bool keyboardContains = containerSprite
        .getTransform()
        .transformRect(keyboard.second.getGlobalBounds())
        .contains(MouseAndKeyboard::instance().getMouseWindowPosition());

    if (keyboardContains)
    {
        setByActiveIndex(static_cast<int>(keyboard.first));
        return;
    }

    const bool xboxContains = containerSprite
        .getTransform()
        .transformRect(xbox.second.getGlobalBounds())
        .contains(MouseAndKeyboard::instance().getMouseWindowPosition());

    if (xboxContains)
    {
        setByActiveIndex(static_cast<int>(xbox.first));
        return;
    }

    const bool dualsenseContains = containerSprite
        .getTransform()
        .transformRect(dualsense.second.getGlobalBounds())
        .contains(MouseAndKeyboard::instance().getMouseWindowPosition());

    if (dualsenseContains)
        setByActiveIndex(static_cast<int>(dualsense.first));
}

void OptionsMenu::ButtonsOverrideOptions::setByActiveIndex(const int activeIndex)
{
    if (activeOption != nullptr)
        activeOption->second.setFillColor(MUTED_TEXT_COLOR);

    activeOptionIndex = activeIndex;
    activeOption = optionPtrs.at(activeOptionIndex);
    activeOption->second.setFillColor(sf::Color::White);
    Input::instance().buttonsOverride = activeOption->first;
}

void OptionsMenu::ButtonsOverrideOptions::setActiveButtonOverrideOption(const int direction)
{
    activeOptionIndex += direction;

    if (activeOptionIndex < 0)
        activeOptionIndex = OPTIONS_COUNT - 1;
    else if (activeOptionIndex > OPTIONS_COUNT - 1)
        activeOptionIndex = 0;

    activeOption->second.setFillColor(MUTED_TEXT_COLOR);
    activeOption = optionPtrs.at(activeOptionIndex);
    activeOption->second.setFillColor(sf::Color::White);
    Input::instance().buttonsOverride = activeOption->first;
}


void OptionsMenu::ButtonsOverrideOptions::draw(sf::RenderTexture& renderTexture)
{
    containerTexture.clear(sf::Color::Transparent);
    containerTexture.draw(label);
    containerTexture.draw(keyboard.second);
    containerTexture.draw(xbox.second);
    containerTexture.draw(dualsense.second);
    containerTexture.display();
    renderTexture.draw(containerSprite);
}


void OptionsMenu::open(IGameState* openedFrom)
{
    this->openedFrom = openedFrom;
    GameRoot::instance().setActiveInputState(&instance());
    GameRoot::instance().addUpdatableState(&instance());
    GameRoot::instance().addDrawableState(&instance());
}


void OptionsMenu::close()
{
    if (activeSliderOption.sliderOption != nullptr)
        activeSliderOption.sliderOption->onRelease();

    GameRoot::instance().setActiveInputState(openedFrom);
    GameRoot::instance().removeUpdatableState(&instance());
    GameRoot::instance().removeDrawableState(&instance());
    openedFrom = nullptr;

    saveOptions();
}


void OptionsMenu::setActiveOption(IOption* option)
{
    activeOption->label.setFillColor(IOption::MUTED_TEXT_COLOR);
    activeOption = option;
    activeOption->label.setFillColor(sf::Color::White);
}


void OptionsMenu::toggleVsync(const bool fromOptions)
{
    if (fromOptions)
        vsync.checked = options.vsync;
    else
        vsync.checked = !vsync.checked;

    GameRoot::instance().renderWindow.setVerticalSyncEnabled(vsync.checked);
}


void OptionsMenu::startSliding(SliderOption* sliderOption)
{
    const bool contains = sliderOption->containerSprite
            .getTransform()
            .transformRect(sliderOption->knob.getGlobalBounds())
            .contains(MouseAndKeyboard::instance().getMouseWindowPosition());

    if (contains)
    {
        activeSliderOption.originalValue = sliderOption->value;
        activeSliderOption.sliderOption = sliderOption;
        activeSliderOption.mouseStartingPosition = MouseAndKeyboard::instance().getMouseWindowPosition();
    }
}


void OptionsMenu::endSliding()
{
    if (activeSliderOption.sliderOption != nullptr)
    {
        activeSliderOption.originalValue = 0.f;
        activeSliderOption.sliderOption = nullptr;
        activeSliderOption.mouseStartingPosition = {};
    }
}


void OptionsMenu::toggleButtonOverride(const bool fromOptions)
{
    if (fromOptions)
        buttonsOverride.checked = options.buttonsOverride;
    else
        buttonsOverride.checked = !buttonsOverride.checked;

    Input::instance().isButtonsOverrideActive = buttonsOverride.checked;
    buttonsOverrideOptions.setByActiveIndex(static_cast<int>(Input::instance().buttonsOverride));

    // Add or remove the button options to the iterable options
    if (buttonsOverride.checked)
        optionPtrs.at(buttonsOverrideOptions.OPTIONS_MENU_INDEX) = &buttonsOverrideOptions;
    else
        optionPtrs.at(buttonsOverrideOptions.OPTIONS_MENU_INDEX) = nullptr;
}


void OptionsMenu::moveToNextMenuOption(const float direction)
{
    if (optionIndicator.isActiveOptionIndicatorTransitioning)
        return;

    // Iterate the active index
    do
    {
        if (direction < 0)
        {
            activeOptionIndex -= 1;

            if (activeOptionIndex < 0)
                activeOptionIndex = OPTIONS_COUNT - 1;
        }
        else
        {
            activeOptionIndex += 1;

            if (activeOptionIndex > OPTIONS_COUNT - 1)
                activeOptionIndex = 0;
        }
    } while (optionPtrs.at(activeOptionIndex) == nullptr);

    // Set the next active menu option
    const sf::Vector2f leftPosition = optionPtrs.at(activeOptionIndex)->containerSprite.getTransform().transformPoint({
        -20.f,
        optionPtrs.at(activeOptionIndex)->containerSprite.getOrigin().y
    });
    const sf::Vector2f rightPosition = optionPtrs.at(activeOptionIndex)->containerSprite.getTransform().transformPoint({
        optionPtrs.at(activeOptionIndex)->containerSprite.getLocalBounds().size.x,
        optionPtrs.at(activeOptionIndex)->containerSprite.getOrigin().y
    });
    optionIndicator.setActive(leftPosition, rightPosition);
    setActiveOption(optionPtrs.at(activeOptionIndex));
}


void OptionsMenu::update()
{
    optionIndicator.update();

    // Update active slide option to follow the mouse
    if (activeSliderOption.sliderOption != nullptr)
    {
        // Get the difference in the original values from when click hold began
        const float diff = MouseAndKeyboard::instance().getMouseWindowPosition().x - activeSliderOption.mouseStartingPosition.x;
        const float norm = diff / activeSliderOption.sliderOption->fill.getTexture().getSize().x * 100;
        const float newValue = activeSliderOption.originalValue + norm;

        // Clamp the new value to min and max, if the falls outside the range, set it to min or max and finish sliding
        if (newValue < activeSliderOption.sliderOption->MIN)
        {
            activeSliderOption.sliderOption->updateValue(activeSliderOption.sliderOption->MIN);
            endSliding();
        }
        else if (newValue > activeSliderOption.sliderOption->MAX)
        {
            activeSliderOption.sliderOption->updateValue(activeSliderOption.sliderOption->MAX);
            endSliding();
        }
        else
        {
            activeSliderOption.sliderOption->updateValue(newValue);
        }
    }
}


void OptionsMenu::renderGaussianBlur()
{
    optionsBackgroundTexture.clear(backgroundColor);
    optionIndicator.draw(optionsBackgroundTexture);
    optionsBackgroundTexture.display();
    GaussianBlur::instance().drawToBase(optionBackground);
}


void OptionsMenu::renderToScreen()
{
    optionsBackgroundTexture.draw(title);
    vsync.draw(optionsBackgroundTexture);
    musicVol.draw(optionsBackgroundTexture);
    sfxVol.draw(optionsBackgroundTexture);
    buttonsOverride.draw(optionsBackgroundTexture);

    if (buttonsOverride.checked)
        buttonsOverrideOptions.draw(optionsBackgroundTexture);

    optionsBackgroundTexture.draw(backText);
    buttons.draw(optionsBackgroundTexture);
    optionsBackgroundTexture.display();
    GameRoot::instance().renderWindow.draw(optionBackground);
}

void OptionsMenu::processMouseMoved(const sf::Event::MouseMoved* mouseMoved)
{
    // For hovering on an option
    if (!optionIndicator.isActiveOptionIndicatorTransitioning && activeSliderOption.sliderOption == nullptr)
        for (int o = 0; o < OPTIONS_COUNT; o++)
            if (optionPtrs.at(o) != nullptr)
            {
                const bool contains = optionBackground
                .getTransform()
                .transformRect(optionPtrs.at(o)->containerSprite.getGlobalBounds())
                .contains(MouseAndKeyboard::instance().getMouseWindowPosition());

                if (!contains)
                    continue;

                // Menu option can be active
                const sf::Vector2f leftPosition = optionPtrs.at(o)->containerSprite.getTransform().transformPoint({
                    -20.f,
                    optionPtrs.at(o)->containerSprite.getOrigin().y
                });
                const sf::Vector2f rightPosition = optionPtrs.at(o)->containerSprite.getTransform().transformPoint({
                    optionPtrs.at(o)->containerSprite.getLocalBounds().size.x,
                    optionPtrs.at(o)->containerSprite.getOrigin().y
                });

                optionIndicator.setActive(leftPosition, rightPosition);
                activeOptionIndex = o;
                setActiveOption(optionPtrs.at(o));
            }
}


void OptionsMenu::processMouseReleased(const sf::Event::MouseButtonReleased* mouseReleased)
{
    if (mouseReleased->button == sf::Mouse::Button::Left)
    {
        if (const auto sliderOption = dynamic_cast<SliderOption*>(activeOption); sliderOption != nullptr)
        {
            sliderOption->onRelease();
            return;
        }

        if (const auto switchOption = dynamic_cast<SwitchOption*>(activeOption); switchOption != nullptr)
        {
            const bool contains = optionBackground
            .getTransform()
            .transformRect(switchOption->containerSprite.getGlobalBounds())
            .contains(MouseAndKeyboard::instance().getMouseWindowPosition());

            if (contains)
                switchOption->onToggle();

            return;
        }

        if (const auto buttonsOverrideOption = dynamic_cast<ButtonsOverrideOptions*>(activeOption); buttonsOverrideOption != nullptr)
            buttonsOverrideOption->checkOptionsSelected();
    }
}


void OptionsMenu::processMousePressed(const sf::Event::MouseButtonPressed* mousePressed)
{
    if (mousePressed->button == sf::Mouse::Button::Left)
    {
        const bool contains = optionBackground
            .getTransform()
            .transformRect(activeOption->containerSprite.getGlobalBounds())
            .contains(MouseAndKeyboard::instance().getMouseWindowPosition());

        // Only slider options use press events
        if (contains)
            if (const auto sliderOption = dynamic_cast<SliderOption*>(activeOption); sliderOption != nullptr)
                sliderOption->onPress();
    }
}


void OptionsMenu::processMouseWheelScrolledEvent(const sf::Event::MouseWheelScrolled* mouseWheelScrolled)
{
    // Only allow scroll wheel if not sliding an option
    if (activeSliderOption.sliderOption == nullptr)
        moveToNextMenuOption(-mouseWheelScrolled->delta);
}


void OptionsMenu::processKeyPressed(const sf::Event::KeyPressed* keyPressed)
{
    if (keyPressed->scancode == sf::Keyboard::Scancode::Left)
    {
        if (const auto sliderOption = dynamic_cast<SliderOption*>(activeOption); sliderOption != nullptr)
            sliderOption->updateValue(sliderOption->value - 5.f);

        if (const auto buttonOverrideOptions = dynamic_cast<ButtonsOverrideOptions*>(activeOption); buttonOverrideOptions != nullptr)
            buttonsOverrideOptions.setActiveButtonOverrideOption(-1);
    }

    if (keyPressed->scancode == sf::Keyboard::Scancode::Right)
    {
        if (const auto sliderOption = dynamic_cast<SliderOption*>(activeOption); sliderOption != nullptr)
            sliderOption->updateValue(sliderOption->value + 5.f);

        if (const auto buttonOverrideOptions = dynamic_cast<ButtonsOverrideOptions*>(activeOption); buttonOverrideOptions != nullptr)
            buttonsOverrideOptions.setActiveButtonOverrideOption(1);
    }
}


void OptionsMenu::processKeyReleased(const sf::Event::KeyReleased* keyReleased)
{
    if (keyReleased->scancode == sf::Keyboard::Scancode::Enter ||
        keyReleased->scancode == sf::Keyboard::Scancode::NumpadEnter ||
        keyReleased->scancode == sf::Keyboard::Scancode::Space)
        if (const auto switchOption = dynamic_cast<SwitchOption*>(activeOption); switchOption != nullptr)
            switchOption->onToggle();

    if (keyReleased->scancode == sf::Keyboard::Scancode::Escape ||
        keyReleased->scancode == sf::Keyboard::Scancode::C)
        close();

    if (keyReleased->scancode == sf::Keyboard::Scancode::Up)
        moveToNextMenuOption(-1.f);

    if (keyReleased->scancode == sf::Keyboard::Scancode::Down)
        moveToNextMenuOption(1.f);
}


void OptionsMenu::processJoystickButtonReleased(const sf::Event::JoystickButtonReleased* joystickButtonReleased)
{
    if (Input::isSecondaryButton(joystickButtonReleased))
        close();

    if (Input::isPrimaryButton(joystickButtonReleased))
        if (const auto switchOption = dynamic_cast<SwitchOption*>(activeOption); switchOption != nullptr)
            switchOption->onToggle();
}


void OptionsMenu::processJoystickAxisMoved(const sf::Event::JoystickMoved* joystickMoved)
{
    if (Input::isDpadY(joystickMoved) && Input::wasDpadMoved(joystickMoved))
        moveToNextMenuOption(-joystickMoved->position);

    if (Input::isLeftThumbstickY(joystickMoved) && Input:: wasLeftThumbstickMoved(joystickMoved))
        moveToNextMenuOption(joystickMoved->position);

    if (Input::isDpadX(joystickMoved) && Input::wasDpadMoved(joystickMoved))
    {
        if (const auto sliderOption = dynamic_cast<SliderOption*>(activeOption); sliderOption != nullptr)
            sliderOption->updateValue(joystickMoved->position > 0.f ? sliderOption->value + 5.f : sliderOption->value - 5.f);

        if (const auto buttonOverrideOptions = dynamic_cast<ButtonsOverrideOptions*>(activeOption); buttonOverrideOptions != nullptr)
            buttonsOverrideOptions.setActiveButtonOverrideOption(joystickMoved->position > 0.f ? 1 : -1);
    }

    if (Input::isLeftThumbstickX(joystickMoved) && Input:: wasLeftThumbstickMoved(joystickMoved))
    {
        if (const auto sliderOption = dynamic_cast<SliderOption*>(activeOption); sliderOption != nullptr)
            sliderOption->updateValue(joystickMoved->position > 0.f ? sliderOption->value + 5.f : sliderOption->value - 5.f);

        if (const auto buttonOverrideOptions = dynamic_cast<ButtonsOverrideOptions*>(activeOption); buttonOverrideOptions != nullptr)
            buttonsOverrideOptions.setActiveButtonOverrideOption(joystickMoved->position > 0.f ? 1 : -1);
    }
}
