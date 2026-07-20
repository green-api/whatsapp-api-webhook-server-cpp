---
name: GREEN API C++ SDK
version: 1.0.0
description: Complete guide for AI agents to write correct C++ code using GREEN-API WhatsApp SDK
author: GREEN-API Team
keywords:
  - green-api
  - whatsapp
  - sdk
  - c++
  - messaging
  - webhook
---

# GREEN API C++ SDK - Agent Skill Guide

## Quick Overview

The **GREEN-API C++ SDK** enables programmatic interaction with WhatsApp through a RESTful API. This guide teaches AI agents to write production-ready code.

### Key Characteristics
- **Two initialization methods**: Direct API parameters or configuration objects
- **Object-oriented design**: Separate classes for different operation groups (Sending, Receiving, Account, Groups, etc.)
- **Namespace**: All classes are in `greenapi` namespace
- **Chat ID format**: Must use `@c.us` suffix for personal chats, `@g.us` for group chats
- **Response type**: All methods return `greenapi::Response` object
- **Authorization requirement**: Instance must be authenticated before most operations

---

## CRITICAL GOTCHAS (Read First!)

### 1. Chat ID Format MUST Include Suffix
```cpp
// ❌ WRONG - Will fail
api.sending.sendMessage(nlohmann::json::parse(R"({
  "chatId": "71234567890"
})");

// ✅ CORRECT - Personal chat
api.sending.sendMessage(nlohmann::json::parse(R"({
  "chatId": "71234567890@c.us"
})");

// ✅ CORRECT - Group chat
api.sending.sendMessage(nlohmann::json::parse(R"({
  "chatId": "123456789-987654321@g.us"
})");
```

### 2. Instance Must Be Authorized
- The instance must be logged in to WhatsApp before sending messages
- Use `getStateInstance()` to check: should return `"authorized"` or `"got qr code"`
- If not authorized:
  - Call `qr()` to get QR code
  - Scan with WhatsApp Business on your phone
  - Wait for authorization

### 3. Message Sending Rate Limiting
- Recommended delay: **3-5 seconds** between messages to avoid spam detection
- Messages sent without delay may be rejected or rate-limited
- Maximum message text: **20,000 characters**
- Emoji and Unicode fully supported

### 4. Webhook vs HTTP Polling Tradeoff
- **Webhook**: Real-time, requires public endpoint (recommended for production)
- **HTTP Polling**: Simpler setup, but wastes resources (polling every 5-60 seconds)

### 5. Response Handling
Always check `success()` method on response:
```cpp
greenapi::Response resp = api.sending.sendMessage(msg);
if (resp.success()) {
    std::cout << "Message sent: " << resp.getResult() << std::endl;
} else {
    std::cerr << "Error: " << resp.getError() << std::endl;
}
```

---

## Initialization

### Method 1: Direct Parameters
```cpp
#include "greenapi.hpp"
using json = nlohmann::json;

int main() {
    greenapi::GreenApi api(
        "https://api.green-api.com",           // apiUrl
        "https://media.green-api.com",         // mediaUrl
        "1101234567",                          // idInstance (from GREEN-API account)
        "aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPq" // apiTokenInstance (from GREEN-API account)
    );
    
    // Now use api.sending, api.receiving, api.account, etc.
    return 0;
}
```

### Method 2: Default Parameters (For Testing Only)
```cpp
greenapi::GreenApi api();  // Uses hardcoded defaults - NOT for production
```

### Instance Check
```cpp
unsigned int count = api.getNumberOfInstances();
std::cout << "Active instances: " << count << std::endl;
```

---

## Method Groups Reference

### 1. **Sending** (`api.sending.*`)
Send messages, files, locations, contacts, polls, and interactive buttons.

**Key methods:**
- `sendMessage(message)` - Text message
- `sendFileByUrl(message)` - File via URL
- `sendFileByUpload(file, text)` - File via form upload
- `uploadFile(file, header)` - Upload file to storage
- `sendLocation(message)` - Location message
- `sendContact(message)` - Contact card
- `sendPoll(message)` - Poll/voting
- `sendInteractiveButtons(message)` - Interactive button messages
- `forwardMessages(message)` - Forward message

**Example:**
```cpp
json msg = json::parse(R"({
  "chatId": "71234567890@c.us",
  "message": "Hello, World!"
})");
greenapi::Response resp = api.sending.sendMessage(msg);
```

→ **See:** [references/sending.md](references/sending.md)

### 2. **Receiving** (`api.receiving.*`)
Receive incoming notifications via HTTP polling.

**Key methods:**
- `receiveNotification(timeout)` - Get one notification (polling, 5-60 seconds)
- `deleteNotification(receiptId)` - Delete received notification
- `downloadFile(message)` - Download file by file data

**Important:** Use webhook endpoint (not SDK method) for production. HTTP polling is inefficient.

**Example (Polling Loop):**
```cpp
while (true) {
    greenapi::Response notif = api.receiving.receiveNotification(30);  // 30 sec timeout
    if (notif.success()) {
        nlohmann::json data = notif.getResult();
        unsigned int receiptId = data.value("receiptId", 0);
        
        // Process notification...
        std::cout << "Got notification: " << data.dump() << std::endl;
        
        // Delete after processing
        api.receiving.deleteNotification(receiptId);
    }
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
```

→ **See:** [references/receiving.md](references/receiving.md)

### 3. **Account** (`api.account.*`)
Manage account settings, authorization, and status.

**Key methods:**
- `getSettings()` - Get account settings
- `setSettings(settings)` - Update settings (webhook URL, notification types)
- `getStateInstance()` - Get auth state ("authorized", "not authorized", "got qr code")
- `getStatusInstance()` - Get socket connection status
- `qr()` - Get QR code for authorization (base64)
- `getAuthorizationCode(phoneNumber)` - Get auth code via SMS alternative
- `logout()` - Logout account
- `reboot()` - Reboot instance

**Example:**
```cpp
greenapi::Response settings = api.account.getSettings();
if (settings.success()) {
    nlohmann::json data = settings.getResult();
    std::cout << "Account: " << data["wid"] << std::endl;
}
```

→ **See:** [references/account.md](references/account.md)

### 4. **Groups** (`api.groups.*`)
Manage group chats, participants, and permissions.

**Key methods:**
- `createGroup(group)` - Create new group (max 1 group per 5 minutes)
- `updateGroupName(group)` - Change group name
- `getGroupData(group)` - Get group info
- `addGroupParticipant(group)` - Add member
- `removeGroupParticipant(group)` - Remove member
- `setGroupAdmin(group)` - Promote to admin
- `removeAdmin(group)` - Demote from admin
- `updateGroupSettings(group)` - Change permissions
- `leaveGroup(group)` - Bot leaves group

**Example:**
```cpp
json grp = json::parse(R"({
  "groupName": "My Group",
  "chatIds": ["71234567890@c.us", "71987654321@c.us"]
})");
greenapi::Response resp = api.groups.createGroup(grp);
```

→ **See:** [references/groups.md](references/groups.md)

### 5. **Journals** (`api.journals.*`)
Retrieve message and call history.

**Key methods:**
- `getChatHistory(message)` - Get messages from chat
- `getMessage(message)` - Get single message
- `lastIncomingMessages(minutes)` - Get incoming messages (default 1440 min = 24h)
- `lastOutgoingMessages(minutes)` - Get sent messages
- `lastIncomingCalls(minutes)` - Get incoming calls
- `lastOutgoingCalls(minutes)` - Get outgoing calls

**Example:**
```cpp
greenapi::Response msgs = api.journals.lastIncomingMessages(60);  // Last hour
if (msgs.success()) {
    for (auto& msg : msgs.getResult()) {
        std::cout << msg.dump() << std::endl;
    }
}
```

→ **See:** [references/journals.md](references/journals.md)

### 6. **Service Methods** (`api.serviceMethods.*`)
General operations like contacts, typing status, chat management.

**Key methods:**
- `checkWhatsapp(phoneNumber)` - Check if number has WhatsApp
- `getContacts()` - Get all contacts
- `getContactInfo(message)` - Get contact details
- `getAvatar(message)` - Get profile picture
- `editMessage(message)` - Edit sent message
- `deleteMessage(message)` - Delete message
- `getChats(count)` - Get chat list
- `archiveChat(message)` - Archive chat
- `unarchiveChat(message)` - Unarchive chat
- `setDisappearingChat(message)` - Set disappearing message timer
- `sendTyping(message)` - Show "typing" indicator

**Example:**
```cpp
greenapi::Response contacts = api.serviceMethods.getContacts();
if (contacts.success()) {
    for (auto& contact : contacts.getResult()) {
        std::cout << contact["id"] << ": " << contact["name"] << std::endl;
    }
}
```

→ **See:** [references/service.md](references/service.md)

### 7. **Queues** (`api.queues.*`)
Manage message send queue.

**Key methods:**
- `showMessagesQueue()` - List pending messages
- `clearMessagesQueue()` - Clear all pending messages

**Example:**
```cpp
greenapi::Response queue = api.queues.showMessagesQueue();
std::cout << "Queued messages: " << queue.getResult().size() << std::endl;
```

→ **See:** [references/queues.md](references/queues.md)

### 8. **Read Mark** (`api.readMark.*`)
Mark messages as read.

**Key methods:**
- `readChat(message)` - Mark chat as read

**Example:**
```cpp
json msg = json::parse(R"({"chatId": "71234567890@c.us"})");
api.readMark.readChat(msg);
```

→ **See:** [references/readmark.md](references/readmark.md)

### 9. **Statuses** (`api.statuses.*`)
Send and manage WhatsApp statuses (stories).

**Key methods:**
- `sendTextStatus(status)` - Post text status
- `sendVoiceStatus(status)` - Post voice status
- `sendMediaStatus(status)` - Post picture/video status
- `deleteStatus(status)` - Delete own status
- `getIncomingStatuses(minutes)` - Get received statuses
- `getOutgoingStatuses(minutes)` - Get posted statuses
- `getStatusStatistic(idMessage)` - Get status views/reactions

**Example:**
```cpp
json status = json::parse(R"({"text": "I am online!"})");
api.statuses.sendTextStatus(status);
```

→ **See:** [references/statuses.md](references/statuses.md)

---

## Common Scenarios

### Scenario 1: Send Simple Text Message
```cpp
#include "greenapi.hpp"
#include <iostream>
#include <thread>
#include <chrono>

using json = nlohmann::json;

int main() {
    greenapi::GreenApi api(
        "https://api.green-api.com",
        "https://media.green-api.com",
        "YOUR_ID_INSTANCE",
        "YOUR_API_TOKEN"
    );
    
    // Check if authorized
    greenapi::Response state = api.account.getStateInstance();
    if (state.success()) {
        std::string status = state.getResult()["stateInstance"];
        if (status == "authorized") {
            // Send message
            json msg = json::parse(R"({
              "chatId": "71234567890@c.us",
              "message": "Hello from C++ SDK!"
            })");
            
            greenapi::Response resp = api.sending.sendMessage(msg);
            if (resp.success()) {
                std::cout << "Message sent with ID: " 
                          << resp.getResult()["idMessage"] << std::endl;
            } else {
                std::cerr << "Error: " << resp.getError() << std::endl;
            }
        } else {
            std::cout << "Instance not authorized. State: " << status << std::endl;
            greenapi::Response qr = api.account.qr();
            if (qr.success()) {
                std::string qrCode = qr.getResult()["qrCode"];
                std::cout << "Scan QR code to authorize" << std::endl;
            }
        }
    }
    
    return 0;
}
```

### Scenario 2: Polling Loop for Incoming Messages
```cpp
#include "greenapi.hpp"
#include <thread>
#include <chrono>
#include <iostream>

using json = nlohmann::json;

int main() {
    greenapi::GreenApi api(
        "https://api.green-api.com",
        "https://media.green-api.com",
        "YOUR_ID_INSTANCE",
        "YOUR_API_TOKEN"
    );
    
    std::cout << "Starting message polling..." << std::endl;
    
    while (true) {
        // Poll with 30 second timeout
        greenapi::Response notif = api.receiving.receiveNotification(30);
        
        if (notif.success()) {
            json data = notif.getResult();
            unsigned int receiptId = data["receiptId"];
            
            // Process based on notification type
            if (data.contains("body")) {
                std::string type = data["body"].value("typeWebhook", "");
                
                if (type == "incomingMessageReceived") {
                    std::string sender = data["body"]["senderData"]["chatId"];
                    std::string messageText = data["body"]["messageData"]["textMessageData"]["textMessage"];
                    
                    std::cout << "Message from " << sender << ": " << messageText << std::endl;
                    
                    // Reply
                    json reply = json::parse(R"({
                      "chatId": "",
                      "message": "Thanks for your message!"
                    })");
                    reply["chatId"] = sender;
                    api.sending.sendMessage(reply);
                }
            }
            
            // Delete notification from queue
            api.receiving.deleteNotification(receiptId);
        }
        
        // Rate limiting
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}
```

### Scenario 3: Send File by URL
```cpp
json fileMsg = json::parse(R"({
  "chatId": "71234567890@c.us",
  "urlFile": "https://example.com/document.pdf",
  "fileName": "document.pdf",
  "caption": "Please review this document"
})");

greenapi::Response resp = api.sending.sendFileByUrl(fileMsg);
```

### Scenario 4: Upload File and Send
```cpp
// Step 1: Upload file
json file = json::parse(R"({"file": ""})");
file["file"] = "/path/to/local/image.jpg";

json header = json::parse(R"({"fileName": "image.jpg"})");

greenapi::Response upload = api.sending.uploadFile(file, header);
if (upload.success()) {
    std::string fileUrl = upload.getResult()["urlFile"];
    
    // Step 2: Send via URL
    json msg = json::parse(R"({
      "chatId": "71234567890@c.us",
      "urlFile": "",
      "fileName": "image.jpg",
      "caption": "My photo"
    })");
    msg["urlFile"] = fileUrl;
    
    api.sending.sendFileByUrl(msg);
}
```

### Scenario 5: Create Group and Add Members
```cpp
json group = json::parse(R"({
  "groupName": "Project Team",
  "chatIds": [
    "71234567890@c.us",
    "71987654321@c.us",
    "71555555555@c.us"
  ]
})");

greenapi::Response created = api.groups.createGroup(group);
if (created.success()) {
    std::string groupId = created.getResult()["chatId"];
    std::cout << "Group created: " << groupId << std::endl;
    
    // Promote one member to admin
    json admin = json::parse(R"({
      "groupId": "",
      "participantChatId": "71234567890@c.us"
    })");
    admin["groupId"] = groupId;
    
    api.groups.setGroupAdmin(admin);
}
```

---

## Response Handling Pattern

All methods return `greenapi::Response`:

```cpp
greenapi::Response resp = api.sending.sendMessage(msg);

// Check success
if (resp.success()) {
    // Get result data
    nlohmann::json result = resp.getResult();
    std::cout << "Success: " << result.dump() << std::endl;
} else {
    // Get error details
    std::string error = resp.getError();
    std::cout << "Error: " << error << std::endl;
}
```

### Response Methods:
- `success()` - Returns bool
- `getResult()` - Returns `nlohmann::json`
- `getError()` - Returns error message string

---

## Best Practices

### 1. Always Check Authorization First
```cpp
auto state = api.account.getStateInstance();
if (state.success() && 
    state.getResult()["stateInstance"] != "authorized") {
    // Handle not authorized
    return;
}
```

### 2. Respect Rate Limits
```cpp
// Between messages: 3-5 seconds minimum
std::this_thread::sleep_for(std::chrono::seconds(4));
api.sending.sendMessage(msg1);

std::this_thread::sleep_for(std::chrono::seconds(4));
api.sending.sendMessage(msg2);
```

### 3. Use Webhook for Production
Instead of polling in a loop, set webhook in account settings:
```cpp
json settings = json::parse(R"({
  "webhookUrl": "https://your-server.com/webhook",
  "webhookUrlToken": "your-secret-token",
  "incomingWebhook": "yes",
  "outgoingMessageWebhook": "yes",
  "outgoingAPIMessageWebhook": "yes",
  "incomingCallWebhook": "yes"
})");

api.account.setSettings(settings);
```

### 4. Handle Quoted Message ID Properly
```cpp
json reply = json::parse(R"({
  "chatId": "71234567890@c.us",
  "message": "Agreed!",
  "quotedMessageId": "FALSE..."  // ID of message to quote
})");

api.sending.sendMessage(reply);
```

### 5. Validate Chat IDs Format
```cpp
bool isValidChatId(const std::string& chatId) {
    return (chatId.find("@c.us") != std::string::npos) ||
           (chatId.find("@g.us") != std::string::npos);
}
```

---

## Error Scenarios & Solutions

| Error | Cause | Solution |
|-------|-------|----------|
| `instance not authorized` | Account not logged in | Call `qr()`, scan, wait for auth |
| `invalid chatId` | Wrong format or missing @c.us/@g.us | Add correct suffix: `"123@c.us"` |
| `message sending is disabled` | Instance settings issue | Enable in dashboard |
| `invalid file url` | File URL unreachable | Verify URL is public and accessible |
| `request timeout` | Slow connection or server overload | Retry with exponential backoff |
| `rate limit` | Too many messages too fast | Add 5+ second delay between sends |

---

## JSON Structure Notes

All message parameters use **`nlohmann::json`** for maximum flexibility:

```cpp
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Method 1: Parse from string
json msg = json::parse(R"({"chatId": "123@c.us", "message": "Hi"})");

// Method 2: Build object
json msg;
msg["chatId"] = "123@c.us";
msg["message"] = "Hi";

// Method 3: Array support
json buttons = json::array();
buttons.push_back({{"type", "url"}, {"buttonText", "Click"}, {"url", "https://..."}});
msg["buttons"] = buttons;
```

---

## See Also

- **Full method reference:** See `references/` folder
  - [Sending Methods](references/sending.md)
  - [Receiving Methods](references/receiving.md)
  - [Account Management](references/account.md)
  - [Group Operations](references/groups.md)
  - [Message History](references/journals.md)
  - [Service Methods](references/service.md)
  - [Queue Management](references/queues.md)
  - [Status Updates](references/statuses.md)
  - [Message Marking](references/readmark.md)

- **Official Documentation:** https://green-api.com/en/docs/api/
- **GitHub Repository:** https://github.com/green-api/whatsapp-api-client-cpp

---

## Version History

- **v1.0.0** (2024-07-20): Initial skill documentation
  - Complete method inventory
  - All 9 method groups covered
  - Common scenarios with runnable examples
  - Best practices and error handling

---

**Last Updated:** 2024-07-20  
**Status:** Production Ready  
**Tested With:** C++11+, nlohmann/json 3.2+
