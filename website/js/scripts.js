var serverUrl;

function setupRadioButtons(chart, callback) {
    var allDisabled = true;
    var buttons = document.getElementsByName('my-checktext');
    buttons.forEach(function (button) {
        var ping = new XMLHttpRequest();
        ping.open("GET", button.getAttribute("ip") + "/status");
        ping.send();
        ping.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                button.parentElement.style.display = "";
                button.addEventListener("click", function () {
                    serverUrl = button.getAttribute("ip");
                    reload(chart);
                    console.log("Server changed to " + serverUrl);
                });
                if (allDisabled) {
                    serverUrl = button.getAttribute("ip");
                    button.checked = true;
                    allDisabled = false;
                    callback();
                }
                console.log(button.getAttribute("ip") + " is online");
            }
        };
        ping.onerror = function () {
            console.log(button.getAttribute("ip") + " is offline");
        };
    });
}

function parseSensorData(data) {
    var units = {
        "temperature": "&deg;C",
        "relHumidity": "%"
    }
    var table = document.getElementById("measurements");
    table.innerHTML = "";
    var object = JSON.parse(data)
    temperatureData = [];
    humidityData = [];
    timeData = [];
    object.forEach(measurement => {
        temperatureData.push(measurement["temperature"]);
        humidityData.push(measurement["relHumidity"]);
        timeData.push(new Date(measurement["time"]).toLocaleTimeString());

        var row = table.insertRow(0);
        for (var key in units) {
            var cell = row.insertCell(-1);
            cell.innerHTML = measurement[key] + units[key];
        }
        var cell = row.insertCell(-1);
        cell.innerHTML = new Date(measurement["time"]).toLocaleTimeString();
    });
}

function getChartConfig() {
    var small = window.innerWidth < 510;
    return {
        markers: {
            size: 5,
            hover: {
                size: 7,
            },
            strokeWidth: 0
        },
        series: [{
            type: 'line',
            name: "Temperature",
            data: temperatureData
        }, {
            type: 'bar',
            name: "Humidity",
            data: humidityData
        }],
        chart: {
            height: 350,
            zoom: {
                enabled: true
            },
            events: {
                beforeZoom: function (ctx) {
                    if (small)
                        ctx.w.config.xaxis.range = undefined
                },
                dataPointSelection: function (event, chartContext, config) {
                    if (!small) return;
                    document.getElementById("selectiondiv").style.height = "80px";

                    setTimeout(function () {
                        var row = document.getElementById("selected");
                        row.innerHTML = "";
                        row.insertCell(-1).innerHTML = "<span>Temperature:</span> <span>" + config.w.config.series[0].data[config.dataPointIndex] + "&deg;C</span>";
                        row.insertCell(-1).innerHTML = "<span>Humidity:</span> <span>" + config.w.config.series[1].data[config.dataPointIndex] + "% r.H.</span>";

                        row.classList.remove("fadeout");
                        row.style.opacity = 1;
                        row.getBoundingClientRect();
                        row.classList.add("fadeout");
                        row.style.opacity = 0;
                    }, 400);
                    setTimeout(function () {
                        document.getElementById("selectiondiv").style.height = "0px";
                        row.innerHTML = "";
                    }, 4000);

                }

            },
            toolbar: {
                tools: {
                    download: !small,
                    selection: false,
                    zoom: false,
                    zoomin: small,
                    zoomout: small,
                    pan: true,
                    reset: true,
                    customIcons: []

                },
                autoSelected: 'pan'
            },
        },
        dataLabels: {
            enabled: false
        },
        stroke: {
            curve: 'smooth'
        },
        grid: {
            row: {
                colors: ['#f3f3f3', 'transparent'], // takes an array which will be repeated on columns
                opacity: 0.5
            }
        },
        xaxis: {
            categories: timeData,
            range: small ? Math.min(temperatureData.length - 1, 4) : undefined,
            tooltip: {
                enabled: false
            }
        },
        yaxis: [{
            title: {
                text: 'Temperature',
            }

        }, {
            opposite: true,
            title: {
                text: 'rel. Humidity'
            }
        }],
        tooltip: small ? {
            enabled: false
        } : {
            enabled: true,
            custom: function ({ series, seriesIndex, dataPointIndex, w }) {
                var container = '<div>';
                var table = document.createElement("table");
                table.style = "width: 170px;";
                var temp = table.insertRow(0);
                temp.insertCell(0).innerHTML = "Temperature: ";
                temp.insertCell(1).innerHTML = series[0][dataPointIndex] + "&deg;C";

                var humidity = table.insertRow(1);
                humidity.insertCell(0).innerHTML = "Humidity: ";
                humidity.insertCell(1).innerHTML = series[1][dataPointIndex] + "% r.H.";
                return table.outerHTML;
            }
        }
    };
}

function reload(chart) {
    var current = new XMLHttpRequest();
    current.open("GET", serverUrl + "/current");
    current.send();
    current.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var object = JSON.parse(this.responseText);
            document.getElementById("temperature").innerHTML = object["temperature"] + "&deg;C";
            document.getElementById("relHumidity").innerHTML = object["relHumidity"] + "%";
        }
    };

    var his = new XMLHttpRequest();
    his.open("GET", serverUrl + "/all");
    his.send();
    his.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            parseSensorData(this.responseText);
            chart.updateOptions(getChartConfig());
        }
    };
}

function loadNotifications() {
    var element = document.getElementById("notification");
    if (localStorage.getItem("key") == null) {
        var button = document.createElement("button");
        button.innerHTML = "Authorize";
        button.setAttribute("onclick", "window.location.assign('./authorize.html')");
        button.classList.add("blue-button");
        element.appendChild(button);
    }

}

function initialize() {
    temperatureData = [];
    humidityData = [];
    timeData = [];
    var chart = new ApexCharts(document.querySelector("#chart"), getChartConfig());
    setupRadioButtons(chart, function () {
        reload(chart);
        chart.render();

        window.addEventListener('resize', function (event) {
            chart.updateOptions(getChartConfig());
        }, true);

        var counter = 0;
        var timeBetweenReloads = 7000;
        var interval = setInterval(function () {
            counter += 1;
            reload(chart);
            if (counter > 1000 * 60 * 15 / timeBetweenReloads) {
                clearInterval(interval);
            }
        }, 7000);
    });

    loadNotifications();
}

initialize();