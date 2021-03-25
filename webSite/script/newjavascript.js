var map;
var current_address = "saudi arabia";
var flag = 0;
var infoWindows = [];
var markers = [];

$(document).ready(function () {

    start();




});
function start() {
    var ua = navigator.userAgent.toLowerCase();
    var android = ua.indexOf("android") > -1;
    if (android) {
        $("#mobileDiv").addClass("visible");
        $("#mobileDiv").removeClass("invisible");


    }


    show_map(current_address, 5);
    get_sites();
    setTimeout(init, 3000);

    setInterval(repeat, 20000);
}

function refresh() {
    $("#refreshB").addClass("RAnim");
    
    


    setTimeout(function () {
        $("#refreshB").removeClass("RAnim");
        init();
    }, 2000);
    

   



}





function removeMobileDiv() {
    $("#mobileDiv").removeClass("visible");
    $("#mobileDiv").addClass("invisible");

}











var menuPointer = 0;
function menuFunction() {
    if (menuPointer == 0) {

        $("#lefty").removeClass("menuAnimation2");
        $("#menuB").removeClass("BAnimation2");
        $("#lefty").addClass("menuAnimation");
        $("#menuB").addClass("BAnimation");


        setTimeout(function () {
            $("#menuB").css("top", "1200%");
            $("#lefty").css("height", "100%");
            $("#leftyContent").addClass("visible");
            $("#leftyContent").removeClass("invisible");

        }, 950);

        menuPointer = 1;


    } else {
        $("#leftyContent").removeClass("visible");
        $("#leftyContent").addClass("invisible");
        $("#lefty").removeClass("menuAnimation");
        $("#menuB").removeClass("BAnimation");
        $("#lefty").addClass("menuAnimation2");
        $("#menuB").addClass("BAnimation2");



        setTimeout(function () {

            $("#menuB").css("top", "0");
            $("#lefty").css("height", "0");
        }, 950);
        menuPointer = 0

    }



}

function init() {

    if (current_address == "saudi arabia") {
        show_map(current_address, 5);

    } else {
        show_map(current_address, 18);

    }

    get_sites();


}


function repeat() {
    $("#loading").addClass("visible");
    $("#loading").removeClass("invisible");


    deleteMarkers();

    get_new_sites();


    setTimeout(function () {
        $("#loading").addClass("invisible");
        $("#loading").removeClass("visible");


    }, 1000);

}




function setMapOnAll(map) {
    for (var i = 0; i < markers.length; i++) {
        markers[i].setMap(map);
    }
}
function clearMarkers() {

    setMapOnAll(null);
}
function deleteMarkers() {
    clearMarkers();
    markers = [];
}

function get_sites() {
    $.ajax({
        method: "POST",
        url: "server/newEmptyPHP1.php",
        dataType: "json",
        data: {type: "tasks"},
        success: function (data) {


            $.each(data, function () {
                var total = 0;
                $.each(this, function () {
                    total += this.av;
                    if (parseFloat(this.av) == 1) {



                        var myLatLng = {lat: parseFloat(this.lng), lng: parseFloat(this.lat)};
                        
                        
                        var desc= this.desc;
                        
                        
                        var geoUri = "http://maps.google.com/maps?q=loc:" + parseFloat(this.lng) + "," + parseFloat(this.lat);
                        var contentString = '<br>';
                                 
                         if(desc=="students parking")
                             contentString+='<img class="conntentCenter" src="image/W4966.jpg"  width=100% <br>';
                         else if(desc=="staff parking")
                             contentString+='<img class="conntentCenter" src="image/W4937.jpg"  width=100% <br>';
                         else
                             contentString+='<h2 class="conntentCenter" >'+desc+'</h2>';
                         
                        contentString+='<h2 class="conntentCenter" ><a   href="' + geoUri + '"> Take Me There !</a> </h2>';
                        contentString+='<div>';
                         
                         
                        var infowindow = new google.maps.InfoWindow({
                            content: contentString,
                            maxWidth: 150
                            
                        });
                        var marker = new google.maps.Marker({
                            position: myLatLng,
                            map: map


                        });
                        marker.setMap(map);
                        window.markers.push(marker);
                        marker.addListener('click', function () {
                            window.infoWindows.push(infowindow);

                            for (var i = 0; i < infoWindows.length; i++) {
                                infoWindows[i].close();

                            }
                            infowindow.open(map, marker);
                            map.setCenter(marker.getPosition());
                            
                            

                        });
                        marker.setAnimation(google.maps.Animation.BOUNCE);

                        if (flag == 0) {
                            current_address = (this.lng + "," + this.lat);
                            flag = 1;
                        }

                        marker.setMap(map);


                    }
                    setTimeout(function () {

                        $("#loading").addClass("invisible");
                        $("#loading").removeClass("visible");

                    }, 2000);
                    if (total == 0) {
                        $("#myp").removeClass("invisible");
                        $("#myp").addClass("visible");

                    } else
                    {
                        $("#myp").addClass("invisible");
                        $("#myp").removeClass("visible");

                    }



                });
            });

        }
    });
}


function get_new_sites() {
    $.ajax({
        method: "POST",
        url: "server/newEmptyPHP1.php",
        dataType: "json",
        data: {type: "tasks"},
        success: function (data) {
            $("#loading").addClass("visible");
            $("#loading").removeClass("invisible");

            jQuery.each(data, function () {
                var total = 0;
                jQuery.each(this, function () {
                    total += this.av;
                    if (parseFloat(this.av) == 1) {



                        var myLatLng = {lat: parseFloat(this.lng), lng: parseFloat(this.lat)};
                        var desc= this.desc;
                        
                        
                        var geoUri = "http://maps.google.com/maps?q=loc:" + parseFloat(this.lng) + "," + parseFloat(this.lat);
                        var contentString = '<br>';
                                 
                         if(desc=="students parking")
                             contentString+='<img class="conntentCenter" src="image/W4966.jpg"  width=100% <br>';
                         else
                             contentString+='<img class="conntentCenter" src="image/W4937.jpg"  width=100% <br>';
                         
                        contentString+='<h2 class="conntentCenter" ><a   href="' + geoUri + '"> Take Me There !</a> </h2>';
                        contentString+='<div>';
                         
                         
                        var infowindow = new google.maps.InfoWindow({
                            content: contentString,
                            maxWidth: 150
                            
                        });
                        var marker = new google.maps.Marker({
                            position: myLatLng,
                            map: map

                        });

                        window.markers.push(marker);
                        marker.addListener('click', function () {
                            window.infoWindows.push(infowindow);

                            for (var i = 0; i < infoWindows.length; i++) {
                                infoWindows[i].close();

                            }
                            infowindow.open(map, marker);
                            map.setCenter(marker.getPosition());

                        });
                        marker.setAnimation(google.maps.Animation.BOUNCE);
                        marker.setMap(map);

                    }
                    if (total == 0) {
                        $("#myp").removeClass("invisible");
                        $("#myp").addClass("visible");

                    } else
                    {
                        $("#myp").addClass("invisible");
                        $("#myp").removeClass("visible");

                    }



                });
            });

        }
    });

}



function show_map(address, zoom) {


    var geocoder = new google.maps.Geocoder(); //create the geocoder object
    if (geocoder) {
        geocoder.geocode({'address': address}, function (results, status) {
            if (status == google.maps.GeocoderStatus.OK) {
                var location = results[0].geometry.location; //get the longtitude and latitude of a point
                var mapProp = {//define the map options
                    center: new google.maps.LatLng(location.lat(), location.lng()),
                    zoom: zoom,
                    mapTypeId: google.maps.MapTypeId.ROADMAP,
                    mapTypeControl: false

                };

                map = new google.maps.Map(document.getElementById('map'), mapProp);



//                var image = {
//                    url: "image/a.png",
//                    size: new google.maps.Size(71, 71),
//                    origin: new google.maps.Point(0, 0),
//                    anchor: new google.maps.Point(17, 34),
//                    scaledSize: new google.maps.Size(25, 25)
//                };
//                var myLatLng = {lat: 17.107675, lng: 42.784699};
//                var marker = new google.maps.Marker({
//                    position: myLatLng,
//                    map: map,
//                    icon: image
//
//
//                });
//                marker.setMap(map);
            }
        });
    }
}

