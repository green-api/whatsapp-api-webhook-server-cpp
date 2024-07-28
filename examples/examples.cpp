#include "user_adapter.h"

// Incoming message. Parameters: 
// [typeWebhook: string, instanceData: object, timestamp: integer, idMessage: string, senderData: object, messageData: object]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/incoming-message/Webhook-IncomingMessageReceived/     
void UserAdapter::onIncomingMessageReceived(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If a webhook is invalid, you could log a reason.
    // Server respondes to client before UserAdapter function. Response based on validation result.
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "error");
        return;
    }

    // You can get request body using bodyStr:
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");

    // Every request contains instanceData
    const auto instanceData = body.bodyJson["instanceData"];
    const auto IdInstance   = instanceData["idInstance"];
    const auto Wid          = instanceData["wid"];
    const auto TypeInstance = instanceData["typeInstance"];

    // Every request contains senderData
    const auto SenderData = body.bodyJson["senderData"];
    const auto ChatId               = SenderData["chatId"];
    const auto Sender               = SenderData["sender"];
    const auto ChatName             = SenderData["chatName"];
    const auto SenderName           = SenderData["senderName"];
    const auto SenderContactName    = SenderData["senderContactName"];
    
    // Every request contains timestamp and idMessage
    const auto Timestamp = body.bodyJson["timestamp"];
    const auto IdMessage = body.bodyJson["idMessage"];

    greenapi::Logger::Log("Webhook data: " + 
        std::string("Webhook fields: {") +
        std::string("timestamp: ")      + nlohmann::to_string(Timestamp) +
        std::string(", idMessage: ")    + nlohmann::to_string(IdMessage) +
        std::string("}, ") +
        std::string("InstanceData: {idInstance: ")     + nlohmann::to_string(IdInstance) +
        std::string(", wid: ")          + nlohmann::to_string(Wid) +
        std::string(", typeInstance: ") + nlohmann::to_string(TypeInstance) +
        std::string("}, ") +
        std::string("SenderData: {chatId: ")    + nlohmann::to_string(ChatId) +
        std::string(", sender: ")               + nlohmann::to_string(Sender) +
        std::string(", chatName: ")             + nlohmann::to_string(ChatName) +
        std::string(", senderName: ")           + nlohmann::to_string(SenderName) +
        std::string(", senderContactName: ")    + nlohmann::to_string(SenderContactName) +
        std::string("}")
    , "info");

    // Every IncomingMessageReceived webhook contains messageData
    const auto messageData = body.bodyJson["messageData"];

    // messageData contains type of message and it's content
    // Remove quotes from messageData["typeMessage"] and convert in to std::string first
    const std::string typeMessage = std::regex_replace(nlohmann::to_string(messageData["typeMessage"]), std::regex("\\\""), "");

    // Incoming text message. View documentation here:
    // https://green-api.com/en/docs/api/receiving/notifications-format/incoming-message/TextMessage/
    if (typeMessage == "textMessage") {
        const auto TextMessageData = messageData["TextMessageData"];
        const auto TextMessage = TextMessageData["textMessage"];    // string: Text message

        // Check value exists for contains(), because this field is optional
        const auto IsTemplateMessage = TextMessageData.contains("isTemplateMessage") ? TextMessageData["isTemplateMessage"] : nullptr;
        
        greenapi::Logger::Log("Message received: " + nlohmann::to_string(TextMessage) + std::string(", it is ") + 
            ((IsTemplateMessage != nullptr && IsTemplateMessage) ?  std::string("a template") : std::string("not a template"))
        , "info");
        return;
    }

    // Incoming text message or URL message. View documentation here:
    // https://green-api.com/en/docs/api/receiving/notifications-format/incoming-message/ExtendedTextMessage/
    if (typeMessage == "extendedTextMessage") {
        const auto ExtendedTextMessageData = messageData["extendedTextMessageData"];

        // Check values exists for contains(), because some fields may be optional
        const auto Text = ExtendedTextMessageData["text"];
        const auto Description          = ExtendedTextMessageData.contains("description")       ? ExtendedTextMessageData["description"] : nullptr;
        const auto Title                = ExtendedTextMessageData.contains("title")             ? ExtendedTextMessageData["title"] : nullptr;
        const auto JpegThumbnail        = ExtendedTextMessageData.contains("jpegThumbnail")     ? ExtendedTextMessageData["jpegThumbnail"] : nullptr;
        const auto IsForwarded          = ExtendedTextMessageData.contains("isForwarded")       ? ExtendedTextMessageData["isForwarded"] : nullptr;
        const auto ForwardingScore      = ExtendedTextMessageData.contains("forwardingScore")   ? ExtendedTextMessageData["forwardingScore"] : nullptr;
        const auto PreviewType          = ExtendedTextMessageData.contains("previewType")       ? ExtendedTextMessageData["previewType"] : nullptr;
        const auto ContainsAutoReply    = ExtendedTextMessageData.contains("containsAutoReply") ? ExtendedTextMessageData["containsAutoReply"] : nullptr;
        const auto MediaType            = ExtendedTextMessageData.contains("mediaType")         ? ExtendedTextMessageData["mediaType"] : nullptr;
        const auto ShowAdAttribution    = ExtendedTextMessageData.contains("showAdAttribution") ? ExtendedTextMessageData["showAdAttribution"] : nullptr;
        const auto SourceId             = ExtendedTextMessageData.contains("sourceId")          ? ExtendedTextMessageData["sourceId"] : nullptr;
        const auto SourceType           = ExtendedTextMessageData.contains("sourceType")        ? ExtendedTextMessageData["sourceType"] : nullptr;
        const auto SourceUrl            = ExtendedTextMessageData.contains("sourceUrl")         ? ExtendedTextMessageData["sourceUrl"] : nullptr;
        const auto ThumbnailUrl         = ExtendedTextMessageData.contains("thumbnailUrl")      ? ExtendedTextMessageData["thumbnailUrl"] : nullptr;

        std::string ForwardedStatus     = IsForwarded       != nullptr ? (IsForwarded ? "true" : "false") : "not provided";
        std::string ForwardingScoreStr  = ForwardingScore   != nullptr ? nlohmann::to_string(ForwardingScore) : "not provided";
        std::string AutoReplyStatus     = ContainsAutoReply != nullptr ? (ContainsAutoReply ? "true" : "false") : "not provided";
        std::string AdAttributionStatus = ShowAdAttribution != nullptr ? (ShowAdAttribution ? "true" : "false") : "not provided";

        greenapi::Logger::Log("Extended text message received: " 
            + std::string("Text: ")                             + nlohmann::to_string(Text)
            + (Description != nullptr ? ", Description: "       + nlohmann::to_string(Description) : "")
            + (Title != nullptr ? ", Title: "                   + nlohmann::to_string(Title) : "")
            + (JpegThumbnail != nullptr ? ", JpegThumbnail: "   + nlohmann::to_string(JpegThumbnail) : "")
            + ", IsForwarded: " + ForwardedStatus
            + ", ForwardingScore: " + ForwardingScoreStr
            + (PreviewType != nullptr ? ", PreviewType: "       + nlohmann::to_string(PreviewType) : "")
            + ", ContainsAutoReply: " + AutoReplyStatus
            + (MediaType != nullptr ? ", MediaType: "           + nlohmann::to_string(MediaType) : "")
            + ", ShowAdAttribution: " + AdAttributionStatus
            + (SourceId != nullptr ? ", SourceId: "         + nlohmann::to_string(SourceId) : "")
            + (SourceType != nullptr ? ", SourceType: "     + nlohmann::to_string(SourceType) : "")
            + (SourceUrl != nullptr ? ", SourceUrl: "       + nlohmann::to_string(SourceUrl) : "")
            + (ThumbnailUrl != nullptr ? ", ThumbnailUrl: " + nlohmann::to_string(ThumbnailUrl) : "")
        , "info");
        return;
    }

    // Incoming image, video, audio, document message. View documentation here:
    // https://green-api.com/en/docs/api/receiving/notifications-format/incoming-message/ImageMessage/
    if (typeMessage == "imageMessage" || typeMessage == "videoMessage" || typeMessage == "documentMessage" || typeMessage == "audioMessage") {
        const auto FileMessageData  = messageData["fileMessageData"];
        
        // Check values exists for contains(), because some fields may be optional
        const auto DownloadUrl      = FileMessageData["downloadUrl"];
        const auto Caption          = FileMessageData.contains("caption")           ? FileMessageData["caption"] : nullptr;
        const auto FileName         = FileMessageData.contains("fileName")          ? FileMessageData["fileName"] : nullptr;
        const auto JpegThumbnail    = FileMessageData.contains("jpegThumbnail")     ? FileMessageData["jpegThumbnail"] : nullptr;
        const auto MimeType         = FileMessageData.contains("mimeType")          ? FileMessageData["mimeType"] : nullptr;
        const auto IsForwarded      = FileMessageData.contains("isForwarded")       ? FileMessageData["isForwarded"] : nullptr;
        const auto ForwardingScore  = FileMessageData.contains("forwardingScore")   ? FileMessageData["forwardingScore"] : nullptr;

        std::string ForwardedStatus     = IsForwarded != nullptr ? (IsForwarded ? "true" : "false") : "not provided";
        std::string ForwardingScoreStr  = ForwardingScore != nullptr ? nlohmann::to_string(ForwardingScore) : "not provided";

        greenapi::Logger::Log("File message received: " 
            + std::string("DownloadUrl: ")                      + nlohmann::to_string(DownloadUrl)
            + (Caption != nullptr ? ", Caption: "               + nlohmann::to_string(Caption) : "")
            + (FileName != nullptr ? ", FileName: "             + nlohmann::to_string(FileName) : "")
            + (JpegThumbnail != nullptr ? ", JpegThumbnail: "   + nlohmann::to_string(JpegThumbnail) : "")
            + (MimeType != nullptr ? ", MimeType: "             + nlohmann::to_string(MimeType) : "")
            + ", IsForwarded: "     + ForwardedStatus
            + ", ForwardingScore: " + ForwardingScoreStr
        , "info");
        return;
    }

    // Incoming location message. View documentation here:
    // https://green-api.com/en/docs/api/receiving/notifications-format/incoming-message/LocationMessage/
    if (typeMessage == "locationMessage") {
        const auto LocationMessageData      = messageData["locationMessageData"];

        // Check values exists for contains(), because some fields may be optional
        const auto NameLocation     = LocationMessageData["nameLocation"];
        const auto Address          = LocationMessageData["address"];
        const auto JpegThumbnail    = LocationMessageData.contains("jpegThumbnail")     ? LocationMessageData["jpegThumbnail"] : nullptr;
        const auto Latitude         = LocationMessageData["latitude"];
        const auto Longitude        = LocationMessageData["longitude"];
        const auto IsForwarded      = LocationMessageData.contains("isForwarded")       ? LocationMessageData["isForwarded"] : nullptr;
        const auto ForwardingScore  = LocationMessageData.contains("forwardingScore")   ? LocationMessageData["forwardingScore"] : nullptr;

        std::string ForwardedStatus     = IsForwarded       != nullptr ? (IsForwarded ? "true" : "false") : "not provided";
        std::string ForwardingScoreStr  = ForwardingScore   != nullptr ? nlohmann::to_string(ForwardingScore) : "not provided";

        greenapi::Logger::Log("Location message received: " 
            + std::string("NameLocation: ")     + nlohmann::to_string(NameLocation)
            + std::string(", Address: ")        + nlohmann::to_string(Address)
            + (JpegThumbnail != nullptr ?       ", JpegThumbnail: " + nlohmann::to_string(JpegThumbnail) : "")
            + std::string(", Latitude: ")       + nlohmann::to_string(Latitude)
            + std::string(", Longitude: ")      + nlohmann::to_string(Longitude)
            + ", IsForwarded: "     + ForwardedStatus
            + ", ForwardingScore: " + ForwardingScoreStr
        , "info");
        return;
    }

    // Incoming contact message. View documentation here:
    // https://green-api.com/en/docs/api/receiving/notifications-format/incoming-message/ContactMessage/
    if (typeMessage == "contactMessage") {
        const auto ContactMessageData = messageData["contactMessageData"];

        // Check values exists for contains(), because some fields may be optional
        const auto DisplayName      = ContactMessageData["displayName"];
        const auto Vcard            = ContactMessageData["vcard"];
        const auto IsForwarded      = ContactMessageData.contains("isForwarded")        ? ContactMessageData["isForwarded"]: nullptr;
        const auto ForwardingScore  = ContactMessageData.contains("forwardingScore")    ? ContactMessageData["forwardingScore"] : nullptr;

        std::string ForwardedStatus     = IsForwarded != nullptr ? (IsForwarded ? "true" : "false") : "not provided";
        std::string ForwardingScoreStr  = ForwardingScore != nullptr ? nlohmann::to_string(ForwardingScore) : "not provided";

        greenapi::Logger::Log("Contact message received: " 
            + std::string("DisplayName: ")  + nlohmann::to_string(DisplayName)
            + std::string(", Vcard: ")      + nlohmann::to_string(Vcard)
            + ", IsForwarded: "             + ForwardedStatus
            + ", ForwardingScore: "         + ForwardingScoreStr
        , "info");
        return;
    }

    // Incoming contacts array message. View documentation here:
    // https://green-api.com/en/docs/api/receiving/notifications-format/incoming-message/ContactsArrayMessage/
    if (typeMessage == "contactsArrayMessage") {
        // messageData in another messageData object only in this typeMessage
        const auto _messageData = messageData["messageData"];

        // Check values exists for contains(), because some fields may be optional
        const auto Contacts         = _messageData["contacts"];
        const auto IsForwarded      = messageData.contains("isForwarded") ? messageData["isForwarded"] : nullptr;
        const auto ForwardingScore  = messageData.contains("forwardingScore") ? messageData["forwardingScore"] : nullptr;

        std::string ForwardedStatus     = IsForwarded != nullptr ? (IsForwarded ? "true" : "false") : "not provided";
        std::string ForwardingScoreStr  = ForwardingScore != nullptr ? nlohmann::to_string(ForwardingScore) : "not provided";

        std::string ContactsLog;
        if (Contacts.is_array()) {
            bool firstTimeLog {true};
            for (const auto& Contact : Contacts) {
                const auto DisplayName = Contact["displayName"];
                const auto Vcard = Contact["vcard"];

                if (!firstTimeLog) {
                    ContactsLog += "; ";
                } else {
                    firstTimeLog = false;
                }
                
                ContactsLog += "DisplayName: " + nlohmann::to_string(DisplayName) + ", Vcard: " + nlohmann::to_string(Vcard);
            }
        }

        greenapi::Logger::Log("Contacts array message received: " 
            + std::string("Contacts: ")     + ContactsLog
            + ", IsForwarded: "             + ForwardedStatus
            + ", ForwardingScore: "         + ForwardingScoreStr
        , "info");
        return;
    }

    // Incoming buttons message. View documentation here:
    // https://green-api.com/en/docs/api/receiving/notifications-format/incoming-message/ButtonsMessage/
    if (typeMessage == "buttonsMessage") {
        const auto ButtonsMessage = messageData["buttonsMessage"];

        // Check values exists for contains(), because some fields may be optional
        const auto ContentText      = ButtonsMessage["contentText"];
        const auto Footer           = ButtonsMessage.contains("footer") ? ButtonsMessage["footer"] : nullptr;
        const auto IsForwarded      = ButtonsMessage.contains("isForwarded") ? ButtonsMessage["isForwarded"] : nullptr;
        const auto ForwardingScore  = ButtonsMessage.contains("forwardingScore") ? ButtonsMessage["forwardingScore"] : nullptr;

        std::string ButtonsLog;
        if (ButtonsMessage.contains("buttons") && ButtonsMessage["buttons"].is_array()) {
            bool firstTimeLog {true};
            for (const auto& Button : ButtonsMessage["buttons"]) {
                const auto ButtonId = Button["buttonId"];
                const auto ButtonText = Button["buttonText"];

                if (!firstTimeLog) {
                    ButtonsLog += "; ";
                } else {
                    firstTimeLog = false;
                }

                ButtonsLog += std::string("ButtonId: ") + nlohmann::to_string(ButtonId)  + ", ButtonText: " + nlohmann::to_string(ButtonText);
            }
        }

        std::string ForwardedStatus = IsForwarded != nullptr ? (IsForwarded ? "true" : "false") : "not provided";
        std::string ForwardingScoreStr = ForwardingScore != nullptr ? nlohmann::to_string(ForwardingScore) : "not provided";

        std::string FooterText = Footer != nullptr ? ", Footer: " + nlohmann::to_string(Footer) : "";

        greenapi::Logger::Log("Buttons message received: " 
            + std::string("ContentText: ") + nlohmann::to_string(ContentText)
            + FooterText
            + ", Buttons: " + ButtonsLog
            + ", IsForwarded: " + ForwardedStatus
            + ", ForwardingScore: " + ForwardingScoreStr
        , "info");
        return;
    }

    // Incoming selection list message. View documentation here:
    // https://green-api.com/en/docs/api/receiving/notifications-format/incoming-message/ListMessage/
    if (typeMessage == "listMessage") {
        const auto ListMessage = messageData["listMessage"];

        const auto ContentText  = ListMessage["contentText"];
        const auto Title        = ListMessage["title"];
        const auto Footer       = ListMessage["footer"];
        const auto ButtonText   = ListMessage["buttonText"];
        const auto Sections      = ListMessage["sections"];

        // Check values exists for contains(), because some fields may be optional
        const auto IsForwarded = ListMessage.contains("isForwarded") ? ListMessage["isForwarded"] : nullptr;
        const auto ForwardingScore = ListMessage.contains("forwardingScore") ? ListMessage["forwardingScore"] : nullptr;

        std::string SectionsLog;
        if (Sections.is_array()) {
            for (const auto& Section : Sections) {
                const auto SectionTitle = Section["title"];
                const auto Rows = Section["rows"];      
                std::string RowsLog;
                if (Rows.is_array()) {
                    bool firstTimeLog {true};
                    for (const auto& Row : Rows) {
                        const auto RowTitle = Row["title"];
                        const auto RowId = Row["rowId"];
                        const auto Description = Row.contains("description") ? Row["description"] : nullptr;

                        if (!firstTimeLog) {
                            RowsLog += "; ";
                        } else {
                            firstTimeLog = true;
                        }
                        
                        RowsLog += "Title: " + nlohmann::to_string(RowTitle) 
                            + ", RowId: " + nlohmann::to_string(RowId)
                            + (Description != nullptr ? ", Description: " + nlohmann::to_string(Description) : "");
                    }
                }
                
                SectionsLog += "SectionTitle: " + nlohmann::to_string(SectionTitle) 
                    + ", Rows: [" + RowsLog + "]; ";
            }
        }

        std::string ForwardedStatus = IsForwarded != nullptr ? (IsForwarded ? "true" : "false") : "not provided";
        std::string ForwardingScoreStr = ForwardingScore != nullptr ? nlohmann::to_string(ForwardingScore) : "not provided";

        greenapi::Logger::Log("List message received: " 
            + std::string("ContentText: ")      + nlohmann::to_string(ContentText)
            + std::string(", Title: ")          + nlohmann::to_string(Title)
            + std::string(", Footer: ")         + nlohmann::to_string(Footer)
            + std::string(", ButtonText: ")     + nlohmann::to_string(ButtonText)
            + std::string(", Sections: ")       + SectionsLog
            + std::string(", IsForwarded: ")    + ForwardedStatus
            + std::string(", ForwardingScore: ") + ForwardingScoreStr
        , "info");
        return;
    }

    // Incoming template buttons message. View documentation here:
    // https://green-api.com/en/docs/api/receiving/notifications-format/incoming-message/TemplateMessage/
    if (typeMessage == "templateMessage") {
        const auto TemplateMessage = messageData["templateMessage"];

        const auto Namespace    = TemplateMessage["namespace"];
        const auto ElementName  = TemplateMessage["elementName"];
        const auto ContentText  = TemplateMessage["contentText"];
        const auto Footer   = TemplateMessage["footer"];
        const auto Buttons  = TemplateMessage["buttons"];
        // Check values exists for contains(), because some fields may be optional
        const auto IsForwarded = TemplateMessage.contains("isForwarded") ? TemplateMessage["isForwarded"] : nullptr;
        const auto ForwardingScore = TemplateMessage.contains("forwardingScore") ? TemplateMessage["forwardingScore"] : nullptr;

        std::string ButtonsLog;
        if (Buttons.is_array()) {
            bool firstTimeLog {true};
            for (const auto& Button : Buttons) {
                const auto Index = Button["index"];
                const auto UrlButton = Button.contains("urlButton") ? Button["urlButton"] : nullptr;
                const auto CallButton = Button.contains("callButton") ? Button["callButton"] : nullptr;
                const auto QuickReplyButton = Button.contains("quickReplyButton") ? Button["quickReplyButton"] : nullptr;

                if (!firstTimeLog) {
                    ButtonsLog += "; ";
                } else {
                    firstTimeLog = false;
                }
                
                ButtonsLog += "Index: " + nlohmann::to_string(Index) 
                    + (UrlButton != nullptr ? ", UrlButton: " + nlohmann::to_string(UrlButton) : "")
                    + (CallButton != nullptr ? ", CallButton: " + nlohmann::to_string(CallButton) : "")
                    + (QuickReplyButton != nullptr ? ", QuickReplyButton: " + nlohmann::to_string(QuickReplyButton) : "");
            }
        }

        greenapi::Logger::Log("Template message received: " 
            + std::string("Namespace: ")        + nlohmann::to_string(Namespace)
            + std::string(", ElementName: ")    + nlohmann::to_string(ElementName)
            + std::string(", ContentText: ")    + nlohmann::to_string(ContentText)
            + std::string(", Footer: ")         + nlohmann::to_string(Footer)
            + std::string(", Buttons: ")        + ButtonsLog
            + std::string(", IsForwarded: ")    + (IsForwarded != nullptr ? (IsForwarded ? "true" : "false") : "not provided")
            + std::string(", ForwardingScore: ") + (ForwardingScore != nullptr ? nlohmann::to_string(ForwardingScore) : "not provided")
        , "info");
        return;
    }

    // Incoming sticker message. View documentation here:
    // https://green-api.com/en/docs/api/receiving/notifications-format/incoming-message/StickerMessage/
    if (typeMessage == "stickerMessage") {
        const auto FileMessageData = messageData["fileMessageData"];

        // Check values exists for contains(), because some fields may be optional
        const auto DownloadUrl      = FileMessageData["downloadUrl"];
        const auto Caption          = FileMessageData.contains("caption")       ? FileMessageData["caption"] : nullptr;
        const auto FileName         = FileMessageData.contains("fileName")      ? FileMessageData["fileName"] : nullptr;
        const auto JpegThumbnail    = FileMessageData.contains("jpegThumbnail") ? FileMessageData["jpegThumbnail"] : nullptr;
        const auto IsAnimated       = FileMessageData.contains("isAnimated")    ? FileMessageData["isAnimated"] : nullptr;
        const auto MimeType         = FileMessageData.contains("mimeType")      ? FileMessageData["mimeType"] : nullptr;
        const auto IsForwarded      = FileMessageData.contains("isForwarded")   ? FileMessageData["isForwarded"] : nullptr;
        const auto ForwardingScore  = FileMessageData.contains("forwardingScore") ? FileMessageData["forwardingScore"] : nullptr;

        std::string ForwardedStatus     = IsForwarded != nullptr        ? (IsForwarded ? "true" : "false") : "not provided";
        std::string ForwardingScoreStr  = ForwardingScore != nullptr    ? nlohmann::to_string(ForwardingScore) : "not provided";
        std::string AnimatedStatus      = IsAnimated != nullptr         ? (IsAnimated ? "animated" : "not animated") : "not provided";

        greenapi::Logger::Log("Sticker message received: " 
            + std::string("DownloadUrl: ")          + nlohmann::to_string(DownloadUrl)
            + (Caption != nullptr ? ", Caption: "   + nlohmann::to_string(Caption) : "")
            + (FileName != nullptr ? ", FileName: " + nlohmann::to_string(FileName) : "")
            + (JpegThumbnail != nullptr ? ", JpegThumbnail: " + nlohmann::to_string(JpegThumbnail) : "")
            + (MimeType != nullptr ? ", MimeType: " + nlohmann::to_string(MimeType) : "")
            + ", IsAnimated: "      + AnimatedStatus
            + ", IsForwarded: "     + ForwardedStatus
            + ", ForwardingScore: " + ForwardingScoreStr
        , "info");
        return;
    }

    // Incoming reaction message. View documentation here:
    // https://green-api.com/en/docs/api/receiving/notifications-format/incoming-message/ReactionMessage/
    if (typeMessage == "reactionMessage") {
        const auto ExtendedTextMessageData = messageData["extendedTextMessageData"];
        
        const auto Reaction = ExtendedTextMessageData["text"];
        
        std::string QuotedMessageId;
        std::string QuotedMessageSender;
        if (messageData.contains("quotedMessage")) {
            const auto QuotedMessage = messageData["quotedMessage"];
            QuotedMessageId = QuotedMessage["stanzaId"];
            QuotedMessageSender = QuotedMessage["participant"];
        }

        greenapi::Logger::Log("Reaction message received: " 
            + std::string("Reaction: ") + nlohmann::to_string(Reaction)
            + (QuotedMessageId.empty()      ? "" : ", QuotedMessageId: " + QuotedMessageId)
            + (QuotedMessageSender.empty()  ? "" : ", QuotedMessageSender: " + QuotedMessageSender)
        , "info");
        return;
    }
    
    // Group invitation incoming message. View documentation here:
    // https://green-api.com/en/docs/api/receiving/notifications-format/incoming-message/GroupInviteMessage/
    if (typeMessage == "groupInviteMessage") {
        const auto GroupInviteMessageData = messageData["groupInviteMessageData"];

        const auto GroupJid         = GroupInviteMessageData["groupJid"];
        const auto InviteCode       = GroupInviteMessageData["inviteCode"];
        const auto InviteExpiration = GroupInviteMessageData["inviteExpiration"];
        const auto GroupName        = GroupInviteMessageData["groupName"];
        // Check values exists for contains(), because some fields may be optional
        const auto Caption          = GroupInviteMessageData.contains("caption")        ? GroupInviteMessageData["caption"] : nullptr;
        const auto Name             = GroupInviteMessageData.contains("name")           ? GroupInviteMessageData["name"] : nullptr;
        const auto JpegThumbnail    = GroupInviteMessageData.contains("jpegThumbnail")  ? GroupInviteMessageData["jpegThumbnail"] : nullptr;

        std::string QuotedMessageId;
        std::string QuotedMessageSender;
        if (messageData.contains("quotedMessage")) {
            const auto QuotedMessage = messageData["quotedMessage"];
            QuotedMessageId = QuotedMessage["stanzaId"];
            QuotedMessageSender = QuotedMessage["participant"];
        }

        greenapi::Logger::Log("Group invite message received: " 
            + std::string("GroupJid: ")             + nlohmann::to_string(GroupJid)
            + std::string(", InviteCode: ")         + nlohmann::to_string(InviteCode)
            + std::string(", InviteExpiration: ")   + nlohmann::to_string(InviteExpiration)
            + std::string(", GroupName: ")          + nlohmann::to_string(GroupName)
            + (Caption != nullptr ? ", Caption: "   + nlohmann::to_string(Caption) : "")
            + (Name != nullptr ? ", Name: "         + nlohmann::to_string(Name) : "")
            + (JpegThumbnail != nullptr     ?       ", JpegThumbnail: " + nlohmann::to_string(JpegThumbnail) : "")
            + (QuotedMessageId.empty()      ? "" :  ", QuotedMessageId: " + QuotedMessageId)
            + (QuotedMessageSender.empty()  ? "" :  ", QuotedMessageSender: " + QuotedMessageSender)
        , "info");
        return;
    }

    // Incoming poll message. View documentation here:
    // https://green-api.com/en/docs/api/receiving/notifications-format/incoming-message/PollMessage/
    if (typeMessage == "pollMessage") {
        const auto PollMessageData = messageData["pollMessageData"];

        // Check values exists for contains(), because some fields may be optional
        const auto Name             = PollMessageData["name"];
        const auto MultipleAnswers  = PollMessageData.contains("multipleAnswers") ? PollMessageData["multipleAnswers"] : nullptr;

        std::string OptionsLog;
        if (PollMessageData.contains("options") && PollMessageData["options"].is_array()) {
            bool firstOption {true};
            for (const auto& Option : PollMessageData["options"]) {
                if (!firstOption) {
                    OptionsLog += "; ";
                }
                if (firstOption) {
                    firstOption = false;
                }
                const auto OptionName = Option["optionName"];
                OptionsLog += nlohmann::to_string(OptionName);
            }
        }

        greenapi::Logger::Log("Poll message received: " 
            + std::string("Name: ")                 + nlohmann::to_string(Name)
            + std::string(", MultipleAnswers: ")    + ((MultipleAnswers != nullptr && MultipleAnswers)? "true" : "false")
            + ", Options: "                         + OptionsLog
        , "info");
        return;
    }

    // Incoming poll update message. View documentation here:
    // https://green-api.com/en/docs/api/receiving/notifications-format/incoming-message/PollUpdateMessage/
    if (typeMessage == "pollUpdateMessage") {
        const auto PollMessageData = messageData["pollMessageData"];

        const auto StanzaId     = PollMessageData["stanzaId"];
        const auto Name         = PollMessageData["name"];
        // Check values exists for contains(), because some fields may be optional
        const auto MultipleAnswers = PollMessageData.contains("multipleAnswers") ? PollMessageData["multipleAnswers"] : nullptr;

        std::string VotesLog;
        if (PollMessageData.contains("votes") && PollMessageData["votes"].is_array()) {
            for (const auto& Vote : PollMessageData["votes"]) {
                const auto OptionName = Vote["optionName"];
                
                std::string VotersLog;
                if (Vote.contains("optionVoters") && Vote["optionVoters"].is_array()) {
                    bool firstVoter {true};
                    for (const auto& Voter : Vote["optionVoters"]) {
                        if (!firstVoter) {
                            VotersLog += "; ";
                        }
                        if (firstVoter) {
                            firstVoter = true;
                        }
                        const auto VoterId = Voter;
                        VotersLog += nlohmann::to_string(VoterId);
                    }
                }

                VotesLog += std::string("OptionName: ") + nlohmann::to_string(OptionName) 
                    + ", Voters: " + VotersLog + "; ";
            }
        }

        greenapi::Logger::Log("Poll update message received: " 
            + std::string("StanzaId: ") + nlohmann::to_string(StanzaId)
            + std::string(", Name: ") + nlohmann::to_string(Name)
            + std::string(", MultipleAnswers: ") + ((MultipleAnswers != nullptr && MultipleAnswers) ? "true" : "false")
            + ", Votes: " + VotesLog
        , "info");
        return;
    }

    // Simple button selection. View documentation here:
    // https://green-api.com/en/docs/api/receiving/notifications-format/selected-buttons/ButtonsResponseMessage/
    if (typeMessage == "buttonsResponseMessage") {
        const auto ButtonsResponseMessage = messageData["buttonsResponseMessage"];
        const auto stanzaId             = ButtonsResponseMessage["stanzaId"];           // string: button message ID
        const auto selectedButtonId     = ButtonsResponseMessage["selectedButtonId"];   // string: selected button identifier
        const auto selectedButtonText   = ButtonsResponseMessage["selectedButtonText"]; // string: selected button text
        greenapi::Logger::Log("Button response received: " 
            + std::string("stanzaId: ")             + nlohmann::to_string(stanzaId) 
            + std::string(", selectedButtonId: ")   + nlohmann::to_string(selectedButtonId) 
            + std::string(", selectedButtonText: ") + nlohmann::to_string(selectedButtonText)
        , "info");
        return;
    }

    // Simple button selection. View documentation here:
    // https://green-api.com/en/docs/api/receiving/notifications-format/selected-buttons/TemplateButtonsReplyMessage/
    if (typeMessage == "templateButtonReplyMessage") {
        const auto ButtonsMessage = messageData["buttonsMessage"];

        const auto ContentText = ButtonsMessage["contentText"];
        // Check values exists for contains(), because some fields may be optional
        const auto Footer           = ButtonsMessage.contains("footer")         ? ButtonsMessage["footer"] : nullptr;
        const auto IsForwarded      = ButtonsMessage.contains("isForwarded")    ? ButtonsMessage["isForwarded"] : nullptr;
        const auto ForwardingScore  = ButtonsMessage.contains("forwardingScore") ? ButtonsMessage["forwardingScore"] : nullptr;

        std::string ButtonsLog;
        if (ButtonsMessage.contains("buttons") && ButtonsMessage["buttons"].is_array()) {
            bool firstTimeLog {true};
            for (const auto& Button : ButtonsMessage["buttons"]) {
                const auto ButtonId = Button["buttonId"];
                const auto ButtonText = Button["buttonText"];

                if (!firstTimeLog) {
                    ButtonsLog += "; ";
                } else {
                    firstTimeLog = false;
                }

                ButtonsLog += std::string("ButtonId: ") + nlohmann::to_string(ButtonId) 
                    + ", ButtonText: " + nlohmann::to_string(ButtonText);
            }
        }

        std::string ForwardedStatus     = IsForwarded != nullptr ? (IsForwarded ? "true" : "false") : "not provided";
        std::string ForwardingScoreStr  = ForwardingScore != nullptr ? nlohmann::to_string(ForwardingScore) : "not provided";

        std::string FooterText = Footer != nullptr ? ", Footer: " + nlohmann::to_string(Footer) : "";

        greenapi::Logger::Log("Buttons message received: " 
            + std::string("ContentText: ") + nlohmann::to_string(ContentText)
            + FooterText
            + ", Buttons: "         + ButtonsLog
            + ", IsForwarded: "     + ForwardedStatus
            + ", ForwardingScore: " + ForwardingScoreStr
        , "info");
        return;
    }

    if (typeMessage == "listResponseMessage") {
        const auto ListResponseMessage = messageData["listResponseMessage"];

        const auto Title                = ListResponseMessage["title"];
        const auto ListType             = ListResponseMessage["listType"];
        const auto SingleSelectReply    = ListResponseMessage["singleSelectReply"];
        const auto StanzaId             = ListResponseMessage["stanzaId"];

        greenapi::Logger::Log("List response message received: " 
            + std::string("Title: ")                + nlohmann::to_string(Title)
            + std::string(", ListType: ")           + nlohmann::to_string(ListType)
            + std::string(", SingleSelectReply: ")  + nlohmann::to_string(SingleSelectReply)
            + std::string(", StanzaId: ")           + nlohmann::to_string(StanzaId)
        , "info");
        return;
    }

}

// Outcoming message (from phone). Parameters:
// [typeWebhook: string, instanceData: object, timestamp: integer, idMessage: string, senderData: object, messageData: object]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/outgoing-message/OutgoingMessage/
void UserAdapter::onOutgoingMessageReceived(greenapi::Response& body) {
    const auto typeWebhook = body.bodyJson["typeWebhook"];
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "info");
        return;
    }
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");
    
    // The format of a message sent fromp hone is identical to incoming message format, 
    // while incoming webhook type takes on the value outgoingMessageReceived.
    // 
    // View void UserAdapter::onIncomingMessageReceived(greenapi::Response& body) for detailed information.
}

// Outcoming message (via API). Parameters:
// [typeWebhook: string, instanceData: object, timestamp: integer, idMessage: string, senderData: object, messageData: object]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/outgoing-message/OutgoingApiMessage/
void UserAdapter::onOutgoingAPIMessageReceived(greenapi::Response& body) {
    const auto typeWebhook = body.bodyJson["typeWebhook"];
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "info");
        return;
    }
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");
    
    // The format of a message, sent via API, is the same as for an incoming message, 
    // and Incoming webhooks type takes on the value outgoingAPIMessageReceived.
    // 
    // View void UserAdapter::onIncomingMessageReceived(greenapi::Response& body) for detailed information.
}

// Status of a previously sent message: sent, delivered, read, etc. Parameters:
// [typeWebhook: string, chatId: string, instanceData: object, timestamp: integer, idMessage: string, status: string, description: string, sendByApi: boolean]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/outgoing-message/OutgoingMessageStatus/
void UserAdapter::onOutgoingMessageStatus(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If a webhook is invalid, you could log a reason.
    // Server respondes to client before UserAdapter function. Response based on validation result.
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "error");
        return;
    }

    // You can get request body using bodyStr:
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");

    // Every request contains instanceData
    const auto instanceData = body.bodyJson["instanceData"];
    const auto IdInstance   = instanceData["idInstance"];
    const auto Wid          = instanceData["wid"];
    const auto TypeInstance = instanceData["typeInstance"];
    
    // Every request contains timestamp, idMessage, status, description, sendByAPI, chatID
    const auto Timestamp = body.bodyJson["timestamp"];
    const auto IdMessage = body.bodyJson["idMessage"];
    const auto Status       = body.bodyJson["status"];
    const auto Description  = body.bodyJson["description"];
    const auto SendByAPI    = body.bodyJson["sendByAPI"];
    const auto ChatID       = body.bodyJson["chatID"];

    greenapi::Logger::Log("Webhook data: " + 
        std::string("Webhook fields: {") +
        std::string("timestamp: ")      + nlohmann::to_string(Timestamp) +
        std::string(", idMessage: ")    + nlohmann::to_string(IdMessage) +
        std::string(", status: ")       + nlohmann::to_string(Status) +
        std::string(", description: ")  + nlohmann::to_string(Description) +
        std::string(", sendByAPI: ")    + ((SendByAPI) ? std::string("true") : std::string("false")) +
        std::string(", chatID: ")       + nlohmann::to_string(ChatID) +
        std::string("}, ") +
        std::string("InstanceData: {idInstance: ")     + nlohmann::to_string(IdInstance) +
        std::string(", wid: ")          + nlohmann::to_string(Wid) +
        std::string(", typeInstance: ") + nlohmann::to_string(TypeInstance) +
        std::string("}, ")
    , "info");

    // Remove any quotes from status before checking it's value
    const std::string status = std::regex_replace(nlohmann::to_string(Status), std::regex("\\\""), "");
    if (status == "sent") {
        greenapi::Logger::Log("Message sent status", "info");
    } else if (status == "delivered") {
        greenapi::Logger::Log("Message delivered status", "info");
    } else if (status == "read") {
        greenapi::Logger::Log("Message read status", "info");
    } else if (status == "failed") {
        greenapi::Logger::Log("Message failed status", "info");
    } else if (status == "noAccount") {
        greenapi::Logger::Log("Message noAccount status", "info");
    } else if (status == "notInGroup") {
        greenapi::Logger::Log("Message notInGroup status", "info");
    } else if (status == "yellowCard") {
        greenapi::Logger::Log("Message yellowCard status", "info");
    }
}

// Instance authorization status data. Parameters:
// [typeWebhook: string, instanceData: object, timestamp: integer, stateInstance: string]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/StateInstanceChanged/
void UserAdapter::onStateInstanceChanged(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If a webhook is invalid, you could log a reason.
    // Server respondes to client before UserAdapter function. Response based on validation result.
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "error");
        return;
    }

    // You can get request body using bodyStr:
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");

    // Every request contains instanceData
    const auto instanceData = body.bodyJson["instanceData"];
    const auto IdInstance   = instanceData["idInstance"];
    const auto Wid          = instanceData["wid"];
    const auto TypeInstance = instanceData["typeInstance"];
    
    // Every request contains timestamp, stateInstance
    const auto Timestamp = body.bodyJson["timestamp"];
    const auto StateInstance = body.bodyJson["stateInstance"];

    greenapi::Logger::Log("Webhook data: " + 
        std::string("Webhook fields: {") +
        std::string("timestamp: ")      + nlohmann::to_string(Timestamp) +
        std::string(", stateInstance: ")    + nlohmann::to_string(StateInstance) +
        std::string("}, ") +
        std::string("InstanceData: {idInstance: ")     + nlohmann::to_string(IdInstance) +
        std::string(", wid: ")          + nlohmann::to_string(Wid) +
        std::string(", typeInstance: ") + nlohmann::to_string(TypeInstance) +
        std::string("}, ")
    , "info");

    // Remove any quotes from stateInstance before checking it's value
    const std::string stateInstance = std::regex_replace(nlohmann::to_string(StateInstance), std::regex("\\\""), "");
    if (stateInstance == "notAuthorized") {
        greenapi::Logger::Log("Instance not authorized", "info");
    } else if (stateInstance == "authorized") {
        greenapi::Logger::Log("Instance authorized", "info");
    } else if (stateInstance == "blocked") {
        greenapi::Logger::Log("Instance banned", "info");
    } else if (stateInstance == "sleepMode") {
        greenapi::Logger::Log("Status is out of date", "info");
    } else if (stateInstance == "starting") {
        greenapi::Logger::Log("The instance is in the process of starting up (service mode).", "info");
    } else if (stateInstance == "yellowCard") {
        greenapi::Logger::Log("Sending messages has been partially or completely suspended on the account due to spamming activity. ", "info");
    }
}

// Information about the contact's avatar based on the requested phone number. Parameters:
// [typeWebhook: string, instanceData: object, timestamp: integer, deviceData: object]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/AvatarInfo/
void UserAdapter::onDeviceInfo(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If a webhook is invalid, you could log a reason.
    // Server respondes to client before UserAdapter function. Response based on validation result.
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "error");
        return;
    }

    // You can get request body using bodyStr:
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");

    // Every request contains instanceData
    const auto instanceData = body.bodyJson["instanceData"];
    const auto IdInstance   = instanceData["idInstance"];
    const auto Wid          = instanceData["wid"];
    const auto TypeInstance = instanceData["typeInstance"];
    
    // Every request contains timestamp
    const auto Timestamp = body.bodyJson["timestamp"];

    // Every request contains avatarInfo
    const auto AvatarInfo = body.bodyJson["avatarInfo"];
    const auto ChatId    = AvatarInfo["chatId"];
    const auto UrlAvatar = AvatarInfo["urlAvatar"];

    greenapi::Logger::Log("Webhook data: " + 
        std::string("Webhook fields: {") +
        std::string("timestamp: ")      + nlohmann::to_string(Timestamp) +
        std::string("}, ") +
        std::string("InstanceData: {idInstance: ")     + nlohmann::to_string(IdInstance) +
        std::string(", wid: ")          + nlohmann::to_string(Wid) +
        std::string(", typeInstance: ") + nlohmann::to_string(TypeInstance) +
        std::string("}, ") +
        std::string("AvatarInfo: {ChatId: ") + nlohmann::to_string(ChatId) +
        std::string(", UrlAvatar: ")         + nlohmann::to_string(UrlAvatar) +
        std::string("}, ")
    , "info");
}

// Appears when there is an incoming call and contains information about the initiator and recipient of the call. Parameters:
// [from: string, typeWebhook: string, instanceData: object, status: string, timestamp: integer, idMessage: string]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/IncomingCall/
void UserAdapter::onIncomingCall(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If a webhook is invalid, you could log a reason.
    // Server respondes to client before UserAdapter function. Response based on validation result.
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "error");
        return;
    }

    // You can get request body using bodyStr:
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");

    // Every request contains instanceData
    const auto instanceData = body.bodyJson["instanceData"];
    const auto IdInstance   = instanceData["idInstance"];
    const auto Wid          = instanceData["wid"];
    const auto TypeInstance = instanceData["typeInstance"];
    
    // Every request contains from, status, timestamp, idMessage
    const auto From          = body.bodyJson["from"];
    const auto Status        = body.bodyJson["status"];
    const auto Timestamp     = body.bodyJson["timestamp"];
    const auto IdMessage     = body.bodyJson["idMessage"];

    greenapi::Logger::Log("Webhook data: " + 
        std::string("Webhook fields: {") +
        std::string("from: ")           + nlohmann::to_string(From) +
        std::string(", status: ")       + nlohmann::to_string(Status) +
        std::string(", timestamp: ")    + nlohmann::to_string(Timestamp) +
        std::string(", idMessage: ")    + nlohmann::to_string(IdMessage) +
        std::string("}, ") +
        std::string("InstanceData: {idInstance: ")     + nlohmann::to_string(IdInstance) +
        std::string(", wid: ")          + nlohmann::to_string(Wid) +
        std::string(", typeInstance: ") + nlohmann::to_string(TypeInstance) +
        std::string("}, ")
    , "info");

    // Remove any quotes from status before checking it's value
    const std::string status = std::regex_replace(nlohmann::to_string(Status), std::regex("\\\""), "");
    if (status == "offer") {
        greenapi::Logger::Log("Incoming call", "info");
    } else if (status == "pickUp") {
        greenapi::Logger::Log("Answered incoming call", "info");
    } else if (status == "hangUp") {
        greenapi::Logger::Log("The recipient of the incoming call did not pick up the phone and hung up, the “Do not disturb” function is activated on the phone", "info");
    } else if (status == "missed") {
        greenapi::Logger::Log("The call initiator canceled the call", "info");
    } else if (status == "declined") {
        greenapi::Logger::Log("Unanswered incoming call", "info");
    }
}

// Socket state data (whether instance is ready to send/receive messages). Parameters:
// [typeWebhook: string, instanceData: object, timestamp: integer, statusInstance: string]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/StatusInstanceChanged/
void UserAdapter::onStatusInstanceChanged(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If a webhook is invalid, you could log a reason.
    // Server respondes to client before UserAdapter function. Response based on validation result.
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "error");
        return;
    }

    // You can get request body using bodyStr:
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");

    // Every request contains instanceData
    const auto instanceData = body.bodyJson["instanceData"];
    const auto IdInstance   = instanceData["idInstance"];
    const auto Wid          = instanceData["wid"];
    const auto TypeInstance = instanceData["typeInstance"];
    
    // Every request contains timestamp, statusInstance
    const auto Timestamp        = body.bodyJson["timestamp"];
    const auto StatusInstance   = body.bodyJson["statusInstance"];

    greenapi::Logger::Log("Webhook data: " + 
        std::string("Webhook fields: {") +
        std::string("timestamp: ")       + nlohmann::to_string(Timestamp) +
        std::string(", statusInstance: ") + nlohmann::to_string(StatusInstance) +
        std::string("}, ") +
        std::string("InstanceData: {idInstance: ")     + nlohmann::to_string(IdInstance) +
        std::string(", wid: ")          + nlohmann::to_string(Wid) +
        std::string(", typeInstance: ") + nlohmann::to_string(TypeInstance) +
        std::string("}, ")
    , "info");

    // Remove any quotes from statusInstance before checking it's value
    const std::string statusInstance = std::regex_replace(nlohmann::to_string(StatusInstance), std::regex("\\\""), "");
    if (statusInstance == "online") {
        greenapi::Logger::Log("Instance can receive/send messages, socket is open", "info");
    } else if (statusInstance == "offline") {
        greenapi::Logger::Log("Instance can't receive/send messages, socet is closed", "info");
    }
}


// Incoming webhook quotaExceeded contains data about exceeding chat limitations on the Developer plan. 
// Parameters: [typeWebhook: string, instanceData: object, timestamp: integer, quotaData: object]
// View documentation here:
// https://green-api.com/en/docs/api/receiving/notifications-format/StatusInstanceChanged/        
void UserAdapter::onQuotaExceeded(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // If a webhook is invalid, you could log a reason.
    // Server respondes to client before UserAdapter function. Response based on validation result.
    if (body.error) {
        greenapi::Logger::Log("Received invalid webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with error: ") + body.bodyStr, "error");
        return;
    }

    // You can get request body using bodyStr:
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");

    // Every request contains instanceData
    const auto instanceData = body.bodyJson["instanceData"];
    const auto IdInstance   = instanceData["idInstance"];
    const auto Wid          = instanceData["wid"];
    const auto TypeInstance = instanceData["typeInstance"];
    
    // Every request contains quotaData
    const auto QuotaData        = body.bodyJson["quotaData"];
    const auto Method       = QuotaData["method"];
    const auto Used         = QuotaData["used"];
    const auto Total        = QuotaData["total"];
    const auto Status       = QuotaData["status"];
    const auto Description  = QuotaData["description"];

    greenapi::Logger::Log("Webhook data: " + 
        std::string("InstanceData: {idInstance: ")     + nlohmann::to_string(IdInstance) +
        std::string(", wid: ")          + nlohmann::to_string(Wid) +
        std::string(", typeInstance: ") + nlohmann::to_string(TypeInstance) +
        std::string("}, ") + 
        std::string("quotaData: {method: ") + nlohmann::to_string(Method) +
        std::string(", used: ")             + nlohmann::to_string(Used) +
        std::string(", total: ")            + nlohmann::to_string(Total) +
        std::string(", status: ")           + nlohmann::to_string(Status) +
        std::string(", description: ")      + nlohmann::to_string(Description) +
        std::string("}, ")
    , "info");
}

// Requests with unknown webhook type will be handled here.
void UserAdapter::onUnknownTypeWebhook(greenapi::Response& body) {
    // Every request contains typeWebhook. Requests are rejected, if no typeWebhook given.
    const auto typeWebhook = body.bodyJson["typeWebhook"];

    // No error given to request, if it's unknown
    greenapi::Logger::Log("Received webhook: " + nlohmann::to_string(typeWebhook) + std::string(" with body: ") + body.bodyStr, "info");
    
    // Write your handler here:
}