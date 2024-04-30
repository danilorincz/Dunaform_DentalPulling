#pragma once

const char* webpageCode = R"=====(
<!DOCTYPE html>
<html>

<head>
    <title>ESP32 Control</title>
    <script>
        function submitForm() {
            var duration = document.getElementById("duration").value;
            window.location.href = "/set_duration?duration=" + duration;
        }
    </script>
</head>

<body>
    <h1>ESP32 Control</h1>
    <form>
        <label for="duration">Select duration:</label>
        <select id="duration" name="duration">
            <option value="16">16 sec</option>
            <option value="30">30 sec</option>
            <option value="45">45 sec</option>
        </select>
        <button type="button" onclick="submitForm()">Submit</button>
    </form>
</body>

</html>)=====";