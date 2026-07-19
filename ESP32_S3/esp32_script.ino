// ============================================
// ESP32-S3 - INTERSTELLAR PIANO v1.4
// ============================================

#include <WiFi.h>
#include <WebServer.h>
#include <HardwareSerial.h>

const char* AP_SSID     = "PIANO_GS";
const char* AP_PASSWORD = "interstellar";

#define MEGA_RX_PIN  16 
#define MEGA_TX_PIN  5  

HardwareSerial MegaSerial(2);
WebServer server(80);

String currentNote = "---";
float currentFreq = 0;
String currentEvent = "";
int activeLeds = 0;        

unsigned long lastEventTime = 0;
const unsigned long EVENT_TIMEOUT = 600; 

void setup() {
  Serial.begin(115200);
  delay(1000);
  MegaSerial.begin(115200, SERIAL_8N1, MEGA_RX_PIN, MEGA_TX_PIN);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  
  server.on("/", handleRoot);
  server.on("/api", handleAPI);
  server.on("/note", handleNote);
  server.on("/melody", handleMelody);
  server.on("/stop", handleStop);
  server.begin();
}

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>🎹 Piano Interstellare</title>
<style>
* { box-sizing: border-box; margin: 0; padding: 0; }
body { font-family: 'Courier New', monospace; background: linear-gradient(135deg, #0a0e27 0%, #1a0e3a 50%, #2a0e4a 100%); color: #e0d0ff; min-height: 100vh; padding: 12px; }
h1 { text-align: center; color: #ffcc00; text-shadow: 0 0 20px #ffcc00; font-size: 1.6em; margin-bottom: 3px; }
.sub { text-align: center; color: #88aacc; font-size: 0.85em; margin-bottom: 12px; }
.display { text-align: center; background: rgba(26, 26, 58, 0.8); border: 2px solid #ffcc00; border-radius: 12px; padding: 12px; margin-bottom: 12px; box-shadow: 0 0 20px rgba(255, 204, 0, 0.3); }
.note-big { font-size: 2.5em; color: #ffcc00; font-weight: bold; text-shadow: 0 0 15px #ffcc00; }
.freq { color: #88aacc; font-size: 0.85em; margin-top: 3px; }
.event { color: #ff66aa; font-size: 0.8em; margin-top: 5px; min-height: 18px; }
.led-bar { display: flex; justify-content: center; gap: 6px; margin: 10px 0; flex-wrap: wrap; }
.led { width: 38px; height: 38px; border-radius: 50%; background: #1a1a2a; border: 2px solid #333; display: flex; align-items: center; justify-content: center; font-size: 0.7em; color: #555; font-weight: bold; transition: all 0.15s; }
.led.on { box-shadow: 0 0 15px currentColor, 0 0 25px currentColor; transform: scale(1.1); }
.led.do.on  { background: #ff3333; color: #fff; border-color: #ff0000; }
.led.re.on  { background: #ff9933; color: #fff; border-color: #ff6600; }
.led.mi.on  { background: #ffee33; color: #000; border-color: #ffdd00; }
.led.fa.on  { background: #33ff33; color: #000; border-color: #00dd00; }
.led.sol.on { background: #3399ff; color: #fff; border-color: #0066ff; }
.led.la.on  { background: #cc33ff; color: #fff; border-color: #9900ff; }
.led.si.on  { background: #ffffff; color: #000; border-color: #cccccc; }
.led.acc.on { background: #ffff00; color: #000; border-color: #ffaa00; }
.keys-grid { display: grid; grid-template-columns: repeat(4, 1fr); gap: 6px; max-width: 600px; margin: 0 auto 12px; }
.key { background: linear-gradient(180deg, #2a2a5a 0%, #1a1a3a 100%); color: #e0d0ff; border: 2px solid #4444aa; border-radius: 8px; padding: 15px 6px; font-size: 1.2em; font-weight: bold; cursor: pointer; user-select: none; -webkit-tap-highlight-color: transparent; transition: all 0.1s; text-align: center; }
.key:active { background: #ffcc00; color: #0a0e27; transform: scale(0.95); }
.key.chord { background: linear-gradient(180deg, #4a2a5a 0%, #2a1a3a 100%); border-color: #aa44aa; color: #ffaaee; }
.key.chord:active { background: #ff66aa; color: #0a0e27; }
.key.special { background: linear-gradient(180deg, #2a4a5a 0%, #1a3a4a 100%); border-color: #44aaaa; color: #aaffff; }
.key.special:active { background: #00ffff; color: #0a0e27; }
.section { max-width: 600px; margin: 0 auto 10px; padding: 10px; background: rgba(26, 26, 58, 0.7); border-radius: 8px; }
.section h3 { color: #ffcc00; margin-bottom: 8px; font-size: 0.9em; text-align: center; }
.btn { border-radius: 6px; padding: 10px 14px; margin: 3px; font-weight: bold; cursor: pointer; font-family: inherit; font-size: 0.85em; border: 2px solid;}
.btn-m { background: #4a2a4a; color: #ffaaee; border-color: #aa44aa; }
.btn-m:active { background: #ff66aa; color: #0a0e27; }
.btn-s { background: #5a2a2a; color: #ff8888; border-color: #aa4444; }
.btn-s:active { background: #ff4444; color: #0a0e27; }
</style>
</head>
<body>
<h1>🎹 Piano Interstellare</h1>
<div class="sub">Dashboard di Controllo (ESP32)</div>

<div class="display">
  <div class="note-big" id="note">---</div>
  <div class="freq" id="freq">Premi un tasto o seleziona una melodia</div>
  <div class="event" id="event"></div>
</div>

<div class="led-bar">
  <div class="led do" id="led0">DO</div>
  <div class="led re" id="led1">RE</div>
  <div class="led mi" id="led2">MI</div>
  <div class="led fa" id="led3">FA</div>
  <div class="led sol" id="led4">SOL</div>
  <div class="led la" id="led5">LA</div>
  <div class="led si" id="led6">SI</div>
  <div class="led acc" id="led7">★</div>
</div>

<div class="keys-grid">
  <div class="key" data-k="1">1<br><small>DO</small></div>
  <div class="key" data-k="2">2<br><small>RE</small></div>
  <div class="key" data-k="3">3<br><small>MI</small></div>
  <div class="key chord" data-k="A">A<br><small>Fa M</small></div>
  <div class="key" data-k="4">4<br><small>FA</small></div>
  <div class="key" data-k="5">5<br><small>SOL</small></div>
  <div class="key" data-k="6">6<br><small>LA</small></div>
  <div class="key chord" data-k="B">B<br><small>Sol M</small></div>
  <div class="key" data-k="7">7<br><small>SI</small></div>
  <div class="key" data-k="8">8<br><small>DO+</small></div>
  <div class="key" data-k="9">9<br><small>RE+</small></div>
  <div class="key chord" data-k="C">C<br><small>La m</small></div>
  <div class="key chord" data-k="*">*<br><small>Do M</small></div>
  <div class="key special" data-k="0">0<br><small>MELODY</small></div>
  <div class="key chord" data-k="#">#<br><small>Re m</small></div>
  <div class="key special" data-k="D">D<br><small>STOP</small></div>
</div>

<div class="section" style="text-align: center;">
  <h3>🎼 Melodie</h3>
  <button class="btn btn-m" onclick="melody(1)">🔔 Fra Martino</button>
  <button class="btn btn-m" onclick="melody(2)">🚀 Interstellar</button>
  <button class="btn btn-s" onclick="stopMelody()">🛑 STOP SUONO</button>
</div>

<script>
function press(k){ fetch('/note?k='+encodeURIComponent(k)); }
function melody(n){ fetch('/melody?id='+n); }
function stopMelody(){ fetch('/stop'); }
document.querySelectorAll('.key').forEach(function(b){
  b.addEventListener('click',function(){ press(this.dataset.k); });
});
function setLed(idx,on){
  var el=document.getElementById('led'+idx);
  if(el) { if(on) el.classList.add('on'); else el.classList.remove('on'); }
}
function poll(){
  fetch('/api').then(r=>r.json()).then(d=>{
    document.getElementById('note').textContent = d.note;
    document.getElementById('freq').textContent = d.freq > 0 ? d.freq.toFixed(1)+' Hz' : '';
    document.getElementById('event').textContent = d.event;
    for(var i=0; i<8; i++) setLed(i, false);
    if(d.leds){ d.leds.forEach(function(idx){ setLed(idx, true); }); }
  }).catch(e=>{});
}
setInterval(poll, 150); 
poll();
</script>
</body>
</html>
)rawliteral";
  server.send(200, "text/html", html);
}

void handleAPI() {
  String ledsJson = "[";
  bool first = true;
  for (int i = 0; i < 8; i++) {
    if (activeLeds & (1 << i)) {
      if (!first) ledsJson += ",";
      ledsJson += i;
      first = false;
    }
  }
  ledsJson += "]";
  String json = "{";
  json += "\"note\":\""; json += currentNote; json += "\",";
  json += "\"freq\":"; json += currentFreq; json += ",";
  json += "\"event\":\""; json += currentEvent; json += "\",";
  json += "\"leds\":"; json += ledsJson;
  json += "}";
  server.send(200, "application/json", json);
}

void handleNote() {
  if (server.hasArg("k")) {
    String k = server.arg("k");
    MegaSerial.print(k);
    MegaSerial.print(F("\n"));
  }
  server.send(200, "text/plain", "OK");
}

void handleMelody() {
  if (server.hasArg("id")) {
    int id = server.arg("id").toInt();
    if (id == 1) MegaSerial.print(F("E\n"));
    else if (id == 2) MegaSerial.print(F("F\n"));
  }
  server.send(200, "text/plain", "Melody");
}

void handleStop() {
  MegaSerial.print(F("D\n"));
  server.send(200, "text/plain", "Stopped");
}

void loop() {
  server.handleClient();
  
  if (millis() - lastEventTime > EVENT_TIMEOUT && (currentNote != "---" || activeLeds != 0)) {
    currentNote = "---"; currentFreq = 0; currentEvent = ""; activeLeds = 0;
  }

  static String megaBuffer = "";
  while (MegaSerial.available()) {
    char c = MegaSerial.read();
    if (c == '\n') {
      megaBuffer.trim();
      if (megaBuffer.startsWith("N,")) {
        int comma1 = megaBuffer.indexOf(',', 2);
        if (comma1 > 0) {
          currentNote = megaBuffer.substring(2, comma1);
          currentFreq = megaBuffer.substring(comma1 + 1).toFloat();
          currentEvent = ""; 
          lastEventTime = millis();
          
          // FIX LED ESP32: Accende il LED in base alla prima lettera, ignorando l'ottava
          activeLeds = 0;
          if (currentNote.startsWith("C")) activeLeds = (1 << 0);
          else if (currentNote.startsWith("D")) activeLeds = (1 << 1);
          else if (currentNote.startsWith("E")) activeLeds = (1 << 2);
          else if (currentNote.startsWith("F")) activeLeds = (1 << 3);
          else if (currentNote.startsWith("G")) activeLeds = (1 << 4);
          else if (currentNote.startsWith("A")) activeLeds = (1 << 5);
          else if (currentNote.startsWith("B")) activeLeds = (1 << 6);
        }
      }
      else if (megaBuffer.startsWith("C,")) {
        currentEvent = "Accordo: " + megaBuffer.substring(2);
        currentNote = "---"; currentFreq = 0;
        activeLeds = (1 << 7); 
        lastEventTime = millis();
      }
      megaBuffer = "";
    } 
    else if (c != '\r') {
      megaBuffer += c;
      if (megaBuffer.length() > 50) megaBuffer = "";
    }
  }
}
