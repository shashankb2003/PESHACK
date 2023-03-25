import {initializeApp} from 'https://www.gstatic.com/firebasejs/9.9.3/firebase-app.js'
import {get, ref, getDatabase, child} from 'https://www.gstatic.com/firebasejs/9.9.3/firebase-database.js'
const firebaseConfig = {
  apiKey: "AIzaSyB9eIkzpFj2DwOvFn529jHl1H5RWGdOJzQ",
  authDomain: "potholes-cde20.firebaseapp.com",
  databaseURL: "https://potholes-cde20-default-rtdb.firebaseio.com",
  projectId: "potholes-cde20",
  storageBucket: "potholes-cde20.appspot.com",
  messagingSenderId: "97717487459",
  appId: "1:97717487459:web:4e8c2c5b130bef01c53556"
};

const fb = initializeApp(firebaseConfig);
const dbRef = ref(getDatabase())

let map;
const ny = { lat: 40.76010, lng: -73.983002 };
const blr = {lat: 12.9716, lng: 77.5946}

function initMap() {
    const location = ny
    map = new google.maps.Map(document.getElementById("map"), {
      mapTypeId: "roadmap",
      zoom: 15,
      center: location,
      mapId: "6d77c92efad4c954"
    });
    map.setTilt(45);
}

function addPhone(location){
  let svg ='<svg width="800px" height="800px" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><path d="M12.2666 21C17.2372 21 21.2666 16.9706 21.2666 12C21.2666 7.02944 17.2372 3 12.2666 3C7.29604 3 3.2666 7.02944 3.2666 12C3.2666 16.9706 7.29604 21 12.2666 21Z" stroke="#000000" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>  <path d="M12.2666 8V12M12.2666 16H12.2766" stroke="#000000" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/></vg>'
  const svgMarker = {
    url: 'data:image/svg+xml;charset=utf-8,' + encodeURIComponent(svg),
    fillColor: "#FC2947",
    fillOpacity: 1.0,
    strokeWeight: 1,
    rotation: 0,
    anchor: new google.maps.Point(0, 20),
  }


const marker = new google.maps.Marker({
icon: svgMarker,
position: location,
map: map,
});
addBuoy({ lat: 40.76020, lng: -73.985002 })
}
window.initMap = initMap();
