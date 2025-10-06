#ifndef WIFIPAGE_H
#define WIFIPAGE_H
#include <ESP8266WiFi.h>

extern String wifiOptions; // biến global lưu danh sách mạng

String generateWiFiPage()
{
    String page = R"rawliteral(

<html>
    <head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>WiFi Config</title>
    <style>
        body {
          background-color: #121212;
          color: #f0f0f0;
          font-family: Arial, sans-serif;
          margin: 0;
          display: flex;
          flex-direction: column;
          align-items: center;
          text-align: center;
          justify-content: top;
          margin-top: 5%;
          min-height: 100vh;
        }
        h2 {
        font-size:38px;
        margin-bottom:80px;
        }
        form {
        display:inline-block;
        background:#1e1e1e;
        padding:20px;
        border-radius:12px;
        }
        select, input[type=text], input[type=password] {
        width:250px;
        padding:10px;
        margin:8px 0;
        border:none;
        border-radius:8px;
        font-size:14px;
        }
        .password-box {
        position:relative;
        display:inline-block;
        width:250px;
        }
        .password-box input {
        width:100%;
        padding-right:40px;
        box-sizing:border-box;
        }
        .toggle-pass {
        position:absolute;
        right:10px;
        top:50%;
        transform:translateY(-50%);
        background:none;
        border:none;
        cursor:pointer;
        width:24px;
        height:24px;
        padding:0;
        }
        .toggle-pass svg {
        fill:#BBBBBB;
        stroke:#fff;
        stroke-width:0;
        width:24px;
        height:24px;
        opacity:1;
        transition:fill 0.2s ease;
        }
        .toggle-pass:hover svg {
        fill:#666666;
        }
        .btn {
        background-color:#43a047;
        color:white;
        border:none;
        padding:12px 25px;
        font-size:16px;
        border-radius:10px;
        cursor:pointer;
        margin-top:10px;
        }
        .btn:hover {
        background-color:#2e7d32;
        }
        .btn-gray {
        background-color:#555;
        }
        .btn-gray:hover {
        background-color:#333;
        }
        .btn:disabled {
        opacity:0.5;
        cursor:not-allowed;
        }
        .warning {
        color:#ff6666;
        font-size:13px;
        margin-top:5px;
        display:none;
        }
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
    .cancel { background: #555; color: white; }
    @keyframes pop {
      from { transform: scale(0.8); opacity: 0; }
      to   { transform: scale(1); opacity: 1; }
    }
    /* Spinner */
    .spinner {
      border: 10px solid #333;
      border-top: 10px solid #43a047;
      border-radius: 50%;
      width: 50px;
      height: 50px;
      animation: spin 1s linear infinite;
      margin: auto;
    }
    @keyframes spin {
      0%   { transform: rotate(0deg); }
      100% { transform: rotate(360deg); }
    }
    </style>
    <script>
    	function showPopup(type) {
          let popup = document.getElementById("popup");
          let text = document.getElementById("popup-text");
          let okBtn = document.getElementById("popup-ok");

          if (type === 'scanw') {
          	text.innerHTML = "⚠️ Scanning WiFi...";
            document.getElementById("popup").style.visibility = "visible";
            window.location.href = '/scan';
             
          } else if (type === 'reset') {
            text.innerHTML = "🚨 Reset device?";
            okBtn.onclick = () => { window.location.href = '/reset'; };
          }

          popup.style.visibility = "visible";
        }

        function closePopup() {
          document.getElementById("popup").style.visibility = "hidden";
        }
        function togglePassword() {
        var passField = document.getElementById("wifi-pass");
        var eyeOpen = document.getElementById("eye-open");
        var eyeSlash = document.getElementById("eye-slash");
        if (passField.type === "password") {
            passField.type = "text";
            eyeOpen.style.display = "none";
            eyeSlash.style.display = "inline";
        } else {
            passField.type = "password";
            eyeOpen.style.display = "inline";
            eyeSlash.style.display = "none";
        }
        }

        function checkPasswordLength() {
        var passField = document.getElementById("wifi-pass");
        var saveBtn = document.getElementById("save-btn");
        var warning = document.getElementById("warning");
        if (passField.value.length >= 8) {
            saveBtn.disabled = false;
            warning.style.display = "none";
        } else {
            saveBtn.disabled = true;
            warning.style.display = "block";
        }
        }
    </script>
    </head>
    <body>
    <h2>WiFi Configuration</h2>
    <form action="/setwifi" method="POST">
        <label>Select WiFi:</label><br>
        <select name="ssid">
        )rawliteral";

    page += wifiOptions;

    page += R"rawliteral(
        </select><br>
        <div class="password-box">
        <input type="password" id="wifi-pass" name="password" placeholder="Enter Password" oninput="checkPasswordLength()">
        <button type="button" class="toggle-pass" onclick="togglePassword()">
            <!-- Eye open -->
            <svg id="eye-open" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 640 512">
            <path d="M320 96C167.6 96 52.6 206.6 25.6 256c27 49.4 142 160 
                    294.4 160s267.4-110.6 294.4-160C587.4 206.6 
                    472.4 96 320 96zm0 272c-61.8 0-112-50.2-112-112s50.2-112 
                    112-112 112 50.2 112 112-50.2 112-112 
                    112zm0-176c-35.3 0-64 28.7-64 64s28.7 
                    64 64 64 64-28.7 64-64-28.7-64-64-64z"/>
            </svg>
            <!-- Eye slash -->
            <svg id="eye-slash" style="display:none" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 640 512">
            <path d="M320 96C167.6 96 52.6 206.6 25.6 256c27 49.4 142 160 
                    294.4 160s267.4-110.6 294.4-160C587.4 206.6 
                    472.4 96 320 96zm0 272c-61.8 0-112-50.2-112-112s50.2-112 
                    112-112 112 50.2 112 112-50.2 112-112 
                    112zm0-176c-35.3 0-64 28.7-64 64s28.7 
                    64 64 64 64-28.7 64-64-28.7-64-64-64z"/>
            <line x1="120" y1="400" x2="520" y2="120"
                    stroke="#bbbbbb" stroke-width="60" stroke-linecap="round"/>
            </svg>
        </button>
        </div>
        <div id="warning" class="warning">Password must be at least 8 characters.</div>
        <br>
        <button type="button" class="btn btn-gray" onclick="showPopup('scanw')">Rescan WiFi</button><br>
        <input type="submit" id="save-btn" class="btn" value="Save & Connect" disabled>
    </form>
    
      <!-- Popup -->
  <div id="popup" class="popup">
    <div class="popup-content">
    <div class="spinner"></div>
      <h3 id="popup-text">⚠️ Are you sure?</h3>
      <style>
        #popup-text {
          font-size: 16px; 
          font-weight: normal;
        }
      </style>
      <div class="popup-buttons">
        
        <button class="cancel" onclick="closePopup()">Cancel</button>
      </div>
    </div>
  </div>
  
    </body>
    </html>

  )rawliteral";

    return page;
}

#endif
