window.onload = function() {
    makeRequest("GET", "/led/state", function() {
        var result = JSON.parse(this.responseText);
        setLampState(result.state ? "on" : "off");
    });

    animationConfigChanged(document.getElementById("animation-config-select"));
};

function toggleLampState(el) {
    var rem = "off", add = "on";
    var state = 1;
    if (el.classList.contains(add)) {
        state = 0;
        add = [rem, rem = add][0];
    }

    el.classList.remove(rem);
    el.classList.add(add);

    makeRequest("PUT", "/led/state/" + state);
}

function setLampState(state) {
    var el = document.getElementById("tsb");
    var rem = state == "on" ? "off" : "on";
    el.classList.remove(rem);
    el.classList.add(add);
}

var lastSingleColorUpdateDate = new Date();
var lastSingleColorUpdateTimer = null;

function updateSingleColor(el) {
    if (lastSingleColorUpdateTimer != null) {
        clearTimeout(lastSingleColorUpdateTimer);
    }

    if ((new Date().getTime() - lastSingleColorUpdateDate.getTime()) > 200) {
        updateSingleColorReq(el.value);
    } else {
        lastSingleColorUpdateTimer = setTimeout(function() {
            updateSingleColorReq(el.value);
        }, 200);
    }
}

function updateSingleColorReq(color) {
    lastSingleColorUpdateDate = new Date();
    makeRequest("PUT", "/settings/single-color/" + color);
}

function makeRequest(type, address, onSuccess, data) {
    var req = new XMLHttpRequest();
    req.open(type, address);

    if (onSuccess != null) {
        req.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
                onSuccess.call(this);
            }
        };
    }

    req.send();
}

function animationConfigChanged(el) {
    var configs = document.querySelectorAll(".animation-config-wrapper > *");
    for (var i = 0; i < configs.length; i++) {
        if (configs[i].id == "tab-animation-" + el.value) {
            configs[i].classList.add("selected");
        } else {
            configs[i].classList.remove("selected");
        }
    }
}