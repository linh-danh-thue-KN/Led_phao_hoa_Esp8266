#ifndef WEBPAGE_H
#define WEBPAGE_H

const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <meta charset="UTF-8">
  <title>ESP8266 Control</title>
  <style>
    body {
      background-color: #121212;
      color: #f0f0f0;
      font-family: Arial, sans-serif;
      margin: 0;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: top;
      margin-top: 5%;
      min-height: 100vh;
    }
    h2 {
      font-size: 32px;
      margin-bottom: 80px;
    }
    .btn {
      display: flex;
      align-items: center;
      justify-content: center;
      gap: 8px;
      width: 240px;
      margin: 12px 0;
      padding: 14px;
      font-size: 18px;
      font-weight: bold;
      border: none;
      border-radius: 12px;
      cursor: pointer;
      color: white;
      transition: transform 0.2s, background 0.3s;
    }
    .btn:hover { transform: scale(1.05); }
    .blue { background: #1e88e5; }
    .blue:hover { background: #1565c0; }
    .green { background: #43a047; }
    .green:hover { background: #2e7d32; }
    .red { background: #e53935; }
    .red:hover { background: #b71c1c; }

    /* Popup */
    .popup {
      position: fixed;
      top: 0; left: 0; right: 0; bottom: 0;
      display: flex;
      align-items: center;
      justify-content: center;
      background: rgba(0,0,0,0.6);
      visibility: hidden;
    }
    .popup-content {
      background: #1e1e1e;
      padding: 25px 30px;
      border-radius: 12px;
      text-align: center;
      color: #f0f0f0;
      max-width: 300px;
      box-shadow: 0 0 15px rgba(0,0,0,0.5);
      animation: pop 0.25s ease-out;
    }
    .popup-content h3 {
      font-size: 20px;
      margin-bottom: 18px;
    }
    .popup-buttons button {
      margin: 8px;
      padding: 10px 18px;
      border: none;
      border-radius: 8px;
      font-size: 16px;
      cursor: pointer;
    }
    .ok { background: #e53935; color: white; }
    .cancel { background: #555; color: white; }
    @keyframes pop {
      from { transform: scale(0.8); opacity: 0; }
      to   { transform: scale(1); opacity: 1; }
    }
  </style>
</head>
<body>
  <h2>⚙️ ESP8266 Settings</h2>
  <button class="btn blue" onclick="window.location.href='/update'">⬆️ Update</button>
  <button class="btn blue" onclick="window.location.href='/wifi'">📶 WiFi Settings</button>
  <button class="btn green" onclick="showPopup('restart')">⚠️ Restart Device</button>
  <button class="btn red" onclick="showPopup('reset')">🚨 Reset Device</button>

  <!-- Popup -->
  <div id="popup" class="popup">
    <div class="popup-content">
      <h3 id="popup-text">⚠️ Are you sure?</h3>
      <div class="popup-buttons">
        <button class="ok" id="popup-ok">OK</button>
        <button class="cancel" onclick="closePopup()">Cancel</button>
      </div>
    </div>
  </div>

  <script>
    function showPopup(type) {
      let popup = document.getElementById("popup");
      let text = document.getElementById("popup-text");
      let okBtn = document.getElementById("popup-ok");

      if (type === 'restart') {
        text.innerHTML = "⚠️ Restart device?";
        okBtn.onclick = () => { window.location.href = '/restart'; };
      } else if (type === 'reset') {
        text.innerHTML = "🚨 Reset device?";
        okBtn.onclick = () => { window.location.href = '/reset'; };
      }

      popup.style.visibility = "visible";
    }

    function closePopup() {
      document.getElementById("popup").style.visibility = "hidden";
    }
  </script>
</body>
</html>

)rawliteral";


const char html_rs[] PROGMEM  = R"rawliteral(
    <html>
    <head>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <meta charset="UTF-8">
      <title>Restarting...</title>
      <style>
        body {
          background-color: #121212;
          color: #f0f0f0;
          font-family: Arial, sans-serif;
          text-align: center;
          margin-top: 15%;
        }
        h2 { font-size: 28px; margin-bottom: 20px; }
        .countdown {
          font-size: 22px;
          margin-top: 15px;
        }
      </style>
      <script>
        var seconds = 10;
        function updateCountdown() {
          document.getElementById("count").innerHTML = seconds;
          if (seconds > 0) {
            seconds--;
            setTimeout(updateCountdown, 1000);
          } else {
            window.location.href = "/";
          }
        }
        window.onload = updateCountdown;
      </script>
    </head>
    <body>
      <h2>Device is restarting...</h2>
      <div class="countdown">Please wait <span id="count">10</span> seconds...</div>
    </body>
    </html>
  )rawliteral";
#endif
