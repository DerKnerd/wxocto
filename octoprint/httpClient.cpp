//
// Created by imanuel on 30.10.21.
//

#include "httpClient.h"
#include "../MainApp.h"

httplib::Client getClient() {
    auto settings = MainApp::getInstance()->GetSettings();
    auto client = httplib::Client(settings.server);
    client.set_default_headers({
                                       {"X-Api-Key", settings.apiKey}
                               });

    return client;
}
