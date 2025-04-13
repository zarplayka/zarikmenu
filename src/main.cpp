#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

// Global variable to toggle infinite jump
static bool g_infiniteJumpEnabled = false;

class $modify(MyPlayerObject, PlayerObject) {
    bool pushButton(PlayerButton button) {
        if (g_infiniteJumpEnabled && button == PlayerButton::Jump) {
            // Temporarily pretend the player is on the ground
            bool originalOnGround = this->m_isOnGround;
            this->m_isOnGround = true;
            bool result = PlayerObject::pushButton(button);
            this->m_isOnGround = originalOnGround; // Restore original value
            return result;
        }
        return PlayerObject::pushButton(button);
    }
};

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }

        // Add a button to the bottom menu
        auto menu = this->getChildByID("bottom-menu");
        if (menu) {
            auto buttonSprite = CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png");
            if (buttonSprite) {
                auto button = CCMenuItemSpriteExtra::create(
                    buttonSprite,
                    this,
                    menu_selector(MyMenuLayer::onToggleInfiniteJump)
                );
                button->setID("infinite-jump-button");
                menu->addChild(button);
                menu->updateLayout();
            } else {
                log::error("Failed to create button sprite");
            }
        } else {
            log::error("Bottom menu not found");
        }

        return true;
    }

    void onToggleInfiniteJump(CCObject*) {
        g_infiniteJumpEnabled = !g_infiniteJumpEnabled;
        log::info("Infinite Jump: {}", g_infiniteJumpEnabled ? "Enabled" : "Disabled");
        // Show an alert to provide feedback
        FLAlertLayer::create(
            "Infinite Jump",
            g_infiniteJumpEnabled ? "Enabled" : "Disabled",
            "OK"
        )->show();
    }
};