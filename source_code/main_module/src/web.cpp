#include "web.h"

AsyncWebServer server(80);
AsyncEventSource events("/events");

static long last_event_ms = 0;

const char *ssid = ENV_WIFI_SSID;
const char *password = ENV_WIFI_PASS;

static bool esp_now_status = false;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">

<head>
    <meta charset="utf-8">
    <title>WebStopwatch</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-QWTKZyjpPEjISv5WaRU9OFeRpok6YctnYmDr5pNlyT2bRjXh0JMhjY6hW+ALEwIH" crossorigin="anonymous">
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Roboto+Mono:ital,wght@0,100..700;1,100..700&display=swap" rel="stylesheet">
    <style>
        body {
            background-color: #000;
            font-family: "Roboto Mono", monospace;
            font-optical-sizing: auto;
            font-weight: normal;
            font-style: normal;
        }

        body>div {
            width: 300px;
        }

        .text-bg-primary {
            background-color: #161357c8 !important;
        }

        .text-bg-info {
            background-color: #713f72 !important;
            border: #a00fa3 solid 2px !important;
            color: #ddd !important;
        }

        .text-bg-warning {
            background-color: #7a6d46 !important;
            border: #ffc107 solid 2px !important;
            color: #ddd !important;
        }

        .text-bg-success {
            background-color: #345345 !important;
            border: #198754 solid 2px !important;
            color: #ddd !important;
        }

        .border-primary {
            border-color: #161357 !important;
        }

        .rounded {
            border-radius: 25px !important;
        }

        .lap-row {
            transition: height 0.1s ease-in-out;
            height: 0;
            overflow: hidden;
        }
    </style>
</head>

<body>
    <div class="m-2 text-bg-primary rounded  border-5 pb-1">
        <div class="row">
            <div class="col-12 text-center">
                <p class="fs-1 mb-0 px-2" id="currentTimer">00:00.000</p>
            </div>
        </div>
        <div id="bestLapContainer"></div>
        <div id="pastLapsContainer"></div>
    </div>

    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-YvpcrYf0tY3lHB60NNkmXc5s9fDVZLESaAA55NDzOxhy9GkcIdslK1eN7N6jIeHz" crossorigin="anonymous"></script>
    <script>

        let timer = undefined;
        let startTime = 0;

        let lapCounter = 0;
        let bestLapTime = 0;
        let lastLapTime = 0;
        let laps = [];

        const maxPastLaps = 5;
        const __slideUp = function (elem, callback) {
            elem.style.height = 0;
            if (callback != undefined && typeof callback == 'function') {
                setTimeout(callback, 150);
            }
        }
        const __slideDown = function (elem) {
            elem.style.height = `${elem.scrollHeight}px`;
        }

        let __decimalToMinSecMs = function (decimal) {
            let min = Math.floor(decimal / 60);
            let sec = Math.floor(decimal % 60);
            let ms = Math.floor((decimal - Math.floor(decimal)) * 1000);
            min = min.toString().padStart(2, '0');
            sec = sec.toString().padStart(2, '0');
            ms = ms.toString().padStart(3, '0');
            return `${min}:${sec}.${ms}`;
        }

        let __getCurrentLapTime = function () {
            return ((Date.now() - startTime) / 1000).toFixed(3);
        }

        let __getLapDelta = function (lapTime, lastLapTime) {
            if (lastLapTime == 0) {
                return -lapTime;
            }
            return (lapTime - lastLapTime).toFixed(3);
        }

        let __decimalToSecMs = function (decimal) {
            let sec = Math.floor(decimal);
            let ms = Math.floor((decimal - Math.floor(decimal)) * 1000);
            ms = ms.toString().padStart(3, '0');
            let str = `${sec}.${ms}`
            return str.substring(0, 5);
        }

        let __addLapRow = function (lapTime, isBestLap) {
            let bestLapContainer = document.getElementById('bestLapContainer');
            let lapContainer = document.getElementById('pastLapsContainer');
            let lapDelta = __getLapDelta(lapTime, lastLapTime);
            let lapDeltaClass = lapDelta >= 0 ? "text-bg-warning" : "text-bg-success";
            let lapDeltaText = (lapDelta >= 0 ? "+" : "-") + __decimalToSecMs(Math.abs(lapDelta));
            let newLap = `  <div class="row lap-row">
                                <div class="col-12 text-sm">
                                    <p class="fs-4 mb-0  px-2 py-1 d-flex align-items-center justify-content-center">
                                        <span class="fs-6 p-1 me-2 fw-medium">LAP `+ (lapCounter + 1).toString().padStart(2, '\xa0') + `</span>
                                        <span class="fs-6 p-1 badge rounded-pill `+ lapDeltaClass + ` me-2 fw-medium text-body-secondary">` + lapDeltaText + `</span>
                                        `+ __decimalToMinSecMs(lapTime) + `
                                    </p>
                                </div>
                            </div>`;
            if (lapContainer.childElementCount >= maxPastLaps) {
                lapContainer.removeChild(lapContainer.firstElementChild);
                lapContainer.innerHTML += newLap;
                lapContainer.lastElementChild.style.transition = 'none';
                lapContainer.lastElementChild.style.height = 'auto';
                __showBestLapSpacer();
                __slideDown(bestLapContainer.firstElementChild);
            } else {
                lapContainer.innerHTML += newLap;
                __slideDown(lapContainer.lastElementChild);
                __showBestLapSpacer();
                __slideDown(bestLapContainer.firstElementChild);
            }
        }

        let __checkBestLap = function (lapTime) {
            if (bestLapTime == 0 || lapTime < bestLapTime) {
                let bestLapContainer = document.getElementById('bestLapContainer');
                let lapContainer = document.getElementById('pastLapsContainer');
                let newBestLap = `  <div class="row lap-row">
                                        <div class="col-12 text-sm">
                                            <p class="fs-4 mb-0  px-2 py-1 d-flex align-items-center justify-content-center">
                                                <span class="fs-6 p-1 me-2 fw-medium">LAP `+ (lapCounter + 1).toString().padStart(2, '\xa0') + `</span>
                                                <span class="fs-6 p-1 badge rounded-pill text-bg-info me-2 fw-medium text-body-secondary">\xa0BEST\xa0</span>
                                                `+ __decimalToMinSecMs(lapTime) + `
                                            </p>
                                            <hr id="bestLapSpacer" class="my-2 mx-5 d-none">
                                        </div>
                                    </div>`;
                if (bestLapContainer.childElementCount > 0) {
                    bestLapContainer.removeChild(bestLapContainer.firstElementChild);
                    bestLapContainer.innerHTML += newBestLap;
                    bestLapContainer.lastElementChild.style.transition = 'none';
                    bestLapContainer.lastElementChild.style.height = 'auto';
                    if (lapContainer.childElementCount >= 0) {
                        __showBestLapSpacer();
                    }
                } else {
                    bestLapContainer.innerHTML = newBestLap;
                    __slideDown(bestLapContainer.firstElementChild);
                }
                bestLapTime = lapTime;
                return true;
            }
            return false;
        }

        let __showBestLapSpacer = function () {
            let bestLapSpacer = document.getElementById('bestLapSpacer');
            if (bestLapSpacer) {
                document.getElementById('bestLapSpacer').classList.remove('d-none');
            }
        }

        let __clearLapRows = function () {
            let bestLapContainer = document.getElementById('bestLapContainer');
            bestLapContainer.innerHTML = '';
            let lapContainer = document.getElementById('pastLapsContainer');
            lapContainer.innerHTML = '';
        }

        let __handleTimer = function () {
            document.getElementById('currentTimer').innerText = __decimalToMinSecMs(__getCurrentLapTime());
        }

        var start = function () {
            console.log('start');
            stop();
            startTime = Date.now();
            timer = setInterval(__handleTimer, 8);
            // __clearLapRows();
            // setTimeout(stop, 5000);
        }

        var stop = function () {
            console.log('stop');
            if(timer != undefined){
                clearInterval(timer);
                timer = undefined;
            }
            startTime = 0;
            lastLapTime = 0;
            bestLapTime = 0;
            document.getElementById('currentTimer').innerText = '00:00.000';
        }

        var lap = function () {
            console.log('lap');
            if (startTime == 0) {
                start();
                return;
            }
            let lapTime = __getCurrentLapTime();
            __checkBestLap(lapTime);
            __addLapRow(lapTime, false);
            lapCounter++;
            lastLapTime = lapTime;
            startTime = Date.now();
        }

        var reset = function () {
            console.log('reset');
            stop();
            __clearLapRows();
            lapCounter = 0;
        }

        if (!!window.EventSource) {
            var source = new EventSource('/events');

            source.addEventListener('start', function (e) {
                start();
            }, false);
            source.addEventListener('stop', function (e) {
                stop();
            }, false);
            source.addEventListener('lap', function (e) {
                lap();
            }, false);
            source.addEventListener('reset', function (e) {
                reset();
            }, false);

        }
    </script>

    </script>
</body>

</html>
)rawliteral";

void data_receive(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  Serial.print("Data received: ");
  // store incomingData into a variable
  uint8_t data;
  memcpy(&data, incomingData, len);
  switch (data) {
    case 27:
      stopwatch_disable_sensor_check();
      stopwatch_lap();
      break;
    case 42:
      stopwatch_disable_sensor_check();
      stopwatch_lap();
      stopwatch_stop();
      break;
  }
  Serial.println(data);
}

bool web_setup() {
  if (WiFi.isConnected()) {
    return true;
  }

  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  //   screen_show_server_info(WiFi.localIP().toString(), (String)WiFi.macAddress(), (String)WiFi.channel(), esp_now_init() != ESP_OK ? "Failed" : "Success");

  esp_now_status = esp_now_init() == ESP_OK;

  esp_now_register_recv_cb(data_receive);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", index_html);
  });

  //? Esto es necesario?
  events.onConnect([](AsyncEventSourceClient *client) {
    if (client->lastId()) {
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    client->send("hello!", NULL, millis(), 10000);
  });

  server.addHandler(&events);

  server.begin();
  
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return false;
  }
  return true;
}

void web_start() {
  if (millis() > last_event_ms + 1000) {
    events.send("start", "start", millis());
    last_event_ms = millis();
  }
}

void web_stop() {
  //   if (millis() > last_event_ms + 1000) {
  events.send("stop", "stop", millis());
  last_event_ms = millis();
  //   }
}

void web_lap() {
  if (millis() > last_event_ms + 1000) {
    events.send("lap", "lap", millis());
    last_event_ms = millis();
  }
}

void web_reset() {
  if (millis() > last_event_ms + 1000) {
    events.send("reset", "reset", millis());
    last_event_ms = millis();
  }
}

bool web_connected() {
  return WiFi.isConnected();
}

String web_get_ip() {
  return WiFi.localIP().toString();
}

String web_get_mac() {
  return (String)WiFi.macAddress();
}

String web_get_channel() {
  return (String)WiFi.channel();
}

String web_get_status() {
  return esp_now_status ? "Success" : "Failed";
}