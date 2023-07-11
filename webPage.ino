//function to send a response with the web page
void sendHttpResponse(WiFiEspClient client){
  //Response code 200
  client.println("HTTP/1.1 200 OK");
  //content type
  client.println("Content-type:text/html");
  client.println();
  
  //content of the web page
  //the content is divided in multilple parts
  client.println(F(R"=====(
    <!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no"/>
    <style>
    
body {
    margin: auto;
    text-align: center;
}

button {
    border-radius:15px;
    border: 1px solid #2980b9;
    background: none;
    padding: 10px 20px;
    font-size: 25px;
    font-family: "montserrat";
    margin: 10px;
    transition: 0.8s;
    cursor:pointer;
    color: #2980b9;
}

button:focus {
  background-color: #E0E0E0;
}

.btn1 {
    transform: rotate(-20deg);
}

.btn2 {
  margin-top: 20px;
  width:150px;
  height:60px;
}

.btn3 {
    transform: rotate(20deg);
}
    )====="));
    client.println(F(R"=====(
      .btn4 {
    transform: rotate(200deg);
}

.btn5 {
  margin-top: 10px;
  width:150px;
  height:60px;
}

.btn6 {
    transform: rotate(-200deg);
}

.btn7 {
    text-align: center;
    margin-top: 10px;
    height:80px;
    width:120px;
}

.speedslider {
  -webkit-appearance: none;
  width: 250px;
  height: 15px;
  outline: none;
  border-radius: 8px;
  opacity: 1;
  background: #d3d3d3;
}

.speedslider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 25px; 
  height: 25px; 
  background: #2980b9;
  border-radius: 50%;
  cursor: pointer;
}

.speedslider::-moz-range-thumb {
  width: 25px; 
  height: 25px; 
  border-radius: 50%;
  background: #2980b9;;
  cursor: pointer; 
}

    )====="));
    client.println(F(R"=====(
      label {
  font-size: 25px;
  color: #2980b9;
}

h1 {
    font-family: Lato;
    font-size:50px;
    
}

    </style>
    <title>RC car</title>
  </head>
  <body>

    <h1>RC CAR</h1>
    <button class="btn1" onclick="leftF()">^</button>
    <button class="btn2" onclick="goForward()">Forward</button>
    <button class="btn3" onclick="rightF()">^</button><br>
    <button class="btn7" onclick="stopCar()">Stop</button><br>
    <button class="btn4" onclick="leftB()">^</button>
    <button class="btn5" onclick="goBackwards()">Backwards</button>
    <button class="btn6" onclick="rightB()">^</button><br><br>
    <label for="speedslider">SPEED</label><br>
    <input type="range" min="1" max="5" value="2" id="speedslider" class="speedslider" onchange="changeSpeed(this)">
    
    )====="));
    client.println(F(R"=====(
        
    <script>
      const xhttp = new XMLHttpRequest();

      function leftF() {
        xhttp.open("GET", "/FL", true);
        xhttp.send();
      }

      function goForward() {
        xhttp.open("GET", "/FORWARD", true);
        xhttp.send();
      }

      function rightF() {
        xhttp.open("GET", "/FR", true);
        xhttp.send();
      }

      function leftB() {
        xhttp.open("GET", "/BL", true);
        xhttp.send();
      }

      function goBackwards() {
        xhttp.open("GET", "/BACKWARDS", true);
        xhttp.send();
      }

      function rightB() {
        xhttp.open("GET", "/BR", true);
        xhttp.send();
      }

      function stopCar() {
        xhttp.open("GET", "/STOP", true);
        xhttp.send();
      }

      function changeSpeed(speed) {
        xhttp.open("GET", "/SPEED" + speed.value, true);
        xhttp.send();
      }

    </script>
  </body>
</html>
    )====="));
  
  // The HTTP response ends with another blank line:
  client.println();
}

//function to send a response with no content
void sendEmptyResponse(WiFiEspClient client){
  //HTTP code 200
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();

  // The HTTP response ends with another blank line:
  client.println();
}