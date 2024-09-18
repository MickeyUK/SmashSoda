#include "SettingsWidget.h"
#include "../ImGui/imform.h"

SettingsWidget::SettingsWidget(Hosting& hosting)
    : _hosting(hosting)
{
    _disableGuideButton = Config::cfg.input.disableGuideButton;
    _disableKeyboard = Config::cfg.input.disableKeyboard;
    _latencyLimitEnabled = Config::cfg.room.latencyLimit;
    _latencyLimitThreshold = Config::cfg.room.latencyLimitThreshold;
    _leaderboardEnabled = false;
    _autoIndex = Config::cfg.input.autoIndex;
    _flashWindow = Config::cfg.general.flashWindow;
    _sfxEnabled = Config::cfg.audio.sfxEnabled;
    _hostBonkProof = Config::cfg.chat.hostBonkProof;
    _ipBan = Config::cfg.general.ipBan;
    _parsecLogs = Config::cfg.general.parsecLogs;
    _blockVPN = Config::cfg.general.blockVPN;

    _microphoneEnabled = Config::cfg.audio.micEnabled;

    _muteTime = Config::cfg.chat.muteTime;
    _autoMute = Config::cfg.chat.autoMute;
    _autoMuteTime = Config::cfg.chat.autoMuteTime;
    _saveChat = false;

    _hotkeyBB = Config::cfg.general.hotkeyBB;
    _hotkeyLock = Config::cfg.general.hotkeyLock;

    _guestSFX = Config::cfg.permissions.guest.useSFX;
    _guestBB = Config::cfg.permissions.guest.useBB;
    _guestControls = Config::cfg.permissions.guest.changeControls;

    _vipSFX = Config::cfg.permissions.vip.useSFX;
    _vipBB = Config::cfg.permissions.vip.useBB;
    _vipControls = Config::cfg.permissions.vip.changeControls;

    _modSFX = Config::cfg.permissions.moderator.useSFX;
    _modBB = Config::cfg.permissions.moderator.useBB;
    _modControls = Config::cfg.permissions.moderator.changeControls;

    _prependPingLimit = false;

    _countries = Countries();

    _socketEnabled = Config::cfg.socket.enabled;
    _socketPort = Config::cfg.socket.port;

    try {
        strcpy_s(_discord, Config::cfg.chat.discord.c_str());
    } catch (const std::exception&) {
        try {
            strcpy_s(_discord, "");
        }
        catch (const std::exception&) {}
    }

    try {
        strcpy_s(_chatbot, Config::cfg.chat.chatbot.c_str());
    }
    catch (const std::exception&) {
        try
        {
            strcpy_s(_chatbot, "");
        }
        catch (const std::exception&) {}
    }

    try {
        strcpy_s(_welcomeMessage, Config::cfg.chat.welcomeMessage.c_str());
    }
    catch (const std::exception&) {
        try
        {
            strcpy_s(_welcomeMessage, "");
        }
        catch (const std::exception&) {}
    }
}

bool SettingsWidget::render()
{
    AppStyle::pushTitle();
    ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2(800, 900));
    ImGui::Begin("Settings", (bool*)0);
    AppStyle::pushInput();

    ImVec2 size = ImGui::GetContentRegionAvail();

    if (ImGui::BeginTabBar("Settings Tabs", ImGuiTabBarFlags_None))
    {
        AppFonts::pushInput();
        AppColors::pushTitle();
        if (ImGui::BeginTabItem("General"))
        {
            ImGui::BeginChild("innerscroll");
            renderGeneral();
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Chat")) {
            ImGui::BeginChild("innerscroll");
            renderChatbot();
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Permissions")) {
            ImGui::BeginChild("innerscroll");
            renderPermissions();
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Hotkeys")) {
            ImGui::BeginChild("innerscroll");
            renderHotkeys();
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        AppColors::pop();
        AppFonts::pop();
        ImGui::EndTabBar();
    }

    AppStyle::pop();
    ImGui::End();
    AppStyle::pop();

    return true;
}

/// <summary>
/// Renders the general settings tab.
/// </summary>
void SettingsWidget::renderGeneral() {

    static ImVec2 size;
    static ImVec2 pos;
    size = ImGui::GetContentRegionAvail();
    pos = ImGui::GetWindowPos();

    ImGui::Dummy(ImVec2(0, 10.0f));

    AppStyle::pushInput();

    AppStyle::pushLabel();
    ImGui::Text("COUNTRY");
    AppStyle::pop();
    ImGui::SetNextItemWidth(size.x);
    if (ImGui::BeginCombo("### Country picker combo", _countries.list[Config::cfg.arcade.countryIndex].second.c_str(), ImGuiComboFlags_HeightLarge)) {
        for (size_t i = 0; i < _countries.list.size(); ++i) {
            bool isSelected = (i == Config::cfg.arcade.countryIndex);
            if (ImGui::Selectable(_countries.list[i].second.c_str(), isSelected)) {
                Config::cfg.arcade.countryIndex = i;
                Config::cfg.Save();
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    AppStyle::pushLabel();
    ImGui::TextWrapped("This will set the country flag on your Soda Arcade posts.");
    AppStyle::pop();

    ImGui::Dummy(ImVec2(0, 10));

    AppStyle::pushLabel();
    ImGui::Text("THEME");
    AppStyle::pop();
    ImGui::SetNextItemWidth(size.x);

    if (Config::cfg.general.theme >= 5) {
        Config::cfg.general.theme = 0;
    }
    if (ImGui::BeginCombo("### Thumbnail picker combo", themes[Config::cfg.general.theme].c_str(), ImGuiComboFlags_HeightLarge)) {
        for (size_t i = 0; i < 5; ++i) {
            bool isSelected = (i == Config::cfg.general.theme);
            if (ImGui::Selectable(themes[i].c_str(), isSelected)) {
				Config::cfg.general.theme = i;
				Config::cfg.Save();
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    AppStyle::pushLabel();
    ImGui::TextWrapped("You will need to restart to see your changes.");
    AppStyle::pop();

    ImGui::Dummy(ImVec2(0, 20.0f));

    if (ImForm::InputCheckbox("Disable Guide Button", _disableGuideButton,
        "The guide button by default often brings up overlays in software, which can cause issues when hosting.")) {
        Config::cfg.input.disableGuideButton = _disableGuideButton;
        Config::cfg.Save();
        _hosting._disableGuideButton = _disableGuideButton;
    }

    if (ImForm::InputCheckbox("Disable Keyboard", _disableKeyboard,
        "Prevents users without gamepads playing with keyboard.")) {
        Config::cfg.input.disableKeyboard = _disableKeyboard;
        Config::cfg.Save();
        _hosting._disableKeyboard = _disableKeyboard;
    }

    /*if (ImForm::InputCheckbox("Enable !bb hotkey (CTRL+B)", _hotkeyBB,
        "Disable this if you have hotkeys that conflict with this.")) {
        Config::cfg.general.hotkeyBB = _hotkeyBB;
        if (_hotkeyBB) {
            RegisterHotKey(NULL, 1, MOD_CONTROL, 0x42);
        } else {
            UnregisterHotKey(NULL, 1);
        }
        Config::cfg.Save();
    }

    if (ImForm::InputCheckbox("Enable !lock hotkey (CTRL+L)", _hotkeyLock,
        "Disable this if you have hotkeys that conflict with this.")) {
        Config::cfg.general.hotkeyLock = _hotkeyLock;
        if (_hotkeyLock) {
            RegisterHotKey(NULL, 2, MOD_CONTROL, 0x4C);
        } else {
            UnregisterHotKey(NULL, 2);
        }
        Config::cfg.Save();
    }*/

    if (ImForm::InputCheckbox("Auto Index Gamepads", _autoIndex,
        "XInput indices will be identified automatically. Beware, this may cause BSOD crashes for some users!")) {
        Config::cfg.input.autoIndex = _autoIndex;
        Config::cfg.Save();
    }

    if (ImForm::InputCheckbox("Parsec Logs", _parsecLogs,
        "When enabled this will print logs from Parsec in the Log window.")) {
        Config::cfg.general.parsecLogs = _parsecLogs;
        Config::cfg.Save();
    }

    if (ImForm::InputCheckbox("IP Address Bans", _ipBan,
        "When banning a user, their IP address will be banned from joining your room also.")) {
        Config::cfg.general.ipBan = _ipBan;
        Config::cfg.Save();
    }

    if (ImForm::InputCheckbox("WebSocket Server", _socketEnabled,
        "When hosting, Smash Soda will create a WebSocket server that sends data to clients. Required for the overlay!")) {
        Config::cfg.socket.enabled = _socketEnabled;
        Config::cfg.Save();
    }

    if (ImForm::InputNumber("WebSocket Port", _socketPort, 0, 65535,
        "The port the WebSocket server will run on.")) {
        Config::cfg.socket.port = _socketPort;
        Config::cfg.Save();
    }

    if (ImForm::InputCheckbox("Block VPNs", _blockVPN,
        "It is advisable to only enable this if you are having issues with trolls, as some users use VPNs legitimately.")) {
        Config::cfg.general.blockVPN = _blockVPN;
        Config::cfg.Save();
    }

    AppStyle::pop();

}

/// <summary>
/// Renders the chatbot options tab.
/// </summary>
void SettingsWidget::renderChatbot() {

    ImGui::Dummy(ImVec2(0, 10.0f));

    /*if (ImForm::InputCheckbox("Disable !sfx", _sfxEnabled,
        "Prevents guests from using these commands in chat.")) {
        Config::cfg.audio.sfxEnabled = _sfxEnabled;
        Config::cfg.Save();
    }*/

    if (ImForm::InputText("CHATBOT NAME", _chatbot,
        "Can give the ChatBot a silly name if you want!")) {
        Config::cfg.chat.chatbot = _chatbot;
        Config::cfg.Save();
        Config::cfg.chatbotName = "[" + Config::cfg.chat.chatbot + "] ";
    }

    if (ImForm::InputTextArea("WELCOME MESSAGE", _welcomeMessage,
        "Joining guests will see this message. Type _PLAYER_ to insert the guest's name in the message.")) {
        Config::cfg.chat.welcomeMessage = _welcomeMessage;
        Config::cfg.Save();
    }

    if (ImForm::InputText("DISCORD INVITE LINK", _discord,
        "Automatically print invite link in chat with !discord")) {
        Config::cfg.chat.discord = _discord;
        Config::cfg.Save();
    }

    if (ImForm::InputCheckbox("Flash Window on Message", _flashWindow,
        "The window will flash when a message is received, when not focused.")) {
        Config::cfg.general.flashWindow = _flashWindow;
        Config::cfg.Save();
    }

    if (ImForm::InputCheckbox("Disable !bonk", _bonkEnabled,
        "Funny at first, but can quickly turn annoying.")) {
        Config::cfg.chat.bonkEnabled = _bonkEnabled;
        Config::cfg.Save();
    }

    ImGui::BeginGroup();
        ImGui::Indent(10);
        AppColors::pushButtonSolid();
        if (ImGui::Button("Refresh SFX List")) {
            Cache::cache.sfxList.init("./SFX/custom/_sfx.json");
        }
        ImGui::PopStyleColor(4);
        //AppColors::pushButton();
	    ImGui::Unindent(10);
	ImGui::EndGroup();

    // if (ImForm::InputCheckbox("Host can't be bonked", _hostBonkProof,
    //     "You DARE bonk the host!?")) {
    //     Config::cfg.chat.hostBonkProof = _hostBonkProof;
    //     Config::cfg.Save();
    // }

}

/// <summary>
/// Renders the chatbot options tab.
/// </summary>
void SettingsWidget::renderPermissions() {

    ImGui::Dummy(ImVec2(0, 10.0f));

    AppStyle::pushTitle();
    ImGui::Text("Regular Guest");
    AppStyle::pop();

    if (ImForm::InputCheckbox("Can use !sfx command", _guestSFX)) {
        Config::cfg.permissions.guest.useSFX = _guestSFX;
        Config::cfg.Save();
    }

    if (ImForm::InputCheckbox("Can use !bb command", _guestBB)) {
        Config::cfg.permissions.guest.useBB = _guestBB;
        Config::cfg.Save();
    }

    if (ImForm::InputCheckbox("Can change keyboard controls", _guestControls)) {
        Config::cfg.permissions.guest.changeControls = _guestControls;
        Config::cfg.Save();
    }

    AppStyle::pushTitle();
    ImGui::Text("VIPs");
    AppStyle::pop();

    if (ImForm::InputCheckbox("Can use !sfx command", _vipSFX)) {
        Config::cfg.permissions.vip.useBB = _vipSFX;
        Config::cfg.Save();
    }

    if (ImForm::InputCheckbox("Can use !bb command", _vipBB)) {
        Config::cfg.permissions.vip.useBB = _vipBB;
        Config::cfg.Save();
    }

    if (ImForm::InputCheckbox("Can change keyboard controls", _vipControls)) {
        Config::cfg.permissions.vip.changeControls = _vipControls;
        Config::cfg.Save();
    }

    AppStyle::pushTitle();
    ImGui::Text("Moderators");
    AppStyle::pop();

    if (ImForm::InputCheckbox("Can use !sfx command", _modSFX)) {
        Config::cfg.permissions.moderator.useSFX = _modSFX;
        Config::cfg.Save();
    }

    if (ImForm::InputCheckbox("Can use !bb command", _modBB)) {
        Config::cfg.permissions.moderator.useBB = _modBB;
        Config::cfg.Save();
    }

    if (ImForm::InputCheckbox("Can change keyboard controls", _modControls)) {
        Config::cfg.permissions.moderator.changeControls = _modControls;
        Config::cfg.Save();
    }

}

/// <summary>
/// Renders the chatbot options tab.
/// </summary>
void SettingsWidget::renderHotkeys() {

    ImGui::Dummy(ImVec2(0, 10.0f));

    // Mapping key
    if (Config::cfg.mapHotkey) {

        AppStyle::pushTitle();
        ImGui::TextWrapped("Press a key to map to the command: %s", Config::cfg.pendingHotkeyCommand.c_str());
        AppStyle::pop();

    }

    // Show hotkey form
    else if (_showHotkeyForm) {
        
        if (ImForm::InputText("CHAT COMMAND", _hotkeyCommand,
            "What is the command you would like to map to this key.")) {
            Config::cfg.pendingHotkeyCommand = _hotkeyCommand;
        }

        ImGui::BeginGroup();
        ImGui::Indent(10);
        AppColors::pushButtonSolid();
        if (ImGui::Button("Set Key") && Config::cfg.pendingHotkeyCommand != "") {
            Config::cfg.SetHotkey();
            _showHotkeyForm = false;
        }
        ImGui::PopStyleColor(4);
        //AppColors::pushButton();
        ImGui::Unindent(10);
        ImGui::EndGroup();

    }
    else {

        ImGui::BeginGroup();
        ImGui::Indent(10);
        AppColors::pushButtonSolid();
        if (ImGui::Button("Add Hotkey")) {
            _showHotkeyForm = true;
        }
        ImGui::PopStyleColor(4);
        //AppColors::pushButton();
        ImGui::Unindent(10);
        ImGui::EndGroup();

        ImGui::Dummy(ImVec2(0, 10));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 10));

        ImGui::BeginChild("hotkeylist");

        // List hotkeys
        for (int i = 0; i < Config::cfg.hotkeys.keys.size(); i++) {

            IconButton::render(AppIcons::trash, AppColors::primary, ImVec2(30, 30));
            if (ImGui::IsItemActive()) {
                Config::cfg.RemoveHotkey(Config::cfg.hotkeys.keys[i].command);
            }

            ImGui::SameLine();
            ImGui::BeginGroup();
            ImGui::Indent(10);
            AppStyle::pushInput();
            ImGui::Text("%s", Config::cfg.hotkeys.keys[i].command.c_str());
            AppStyle::pop();
            AppStyle::pushLabel();
            //ImGui::Text("%s", path.c_str());
            AppStyle::pop();
            ImGui::Unindent(10);
            ImGui::EndGroup();

            ImGui::Dummy(ImVec2(0, 5));

        }

        ImGui::EndChild();

    }

}