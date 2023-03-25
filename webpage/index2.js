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
const dbRef = ref(getDatabase());


let map;
const ny = { lat: 40.76010, lng: -73.983002 };
const blr = {lat: 12.935222, lng:77.535481}

function initMap() {
      const location = blr
      map = new google.maps.Map(document.getElementById("map"), {
      mapTypeId: "roadmap",
      zoom: 15,
      center: location,
      mapId: "6d77c92efad4c954"
    });
    map.setTilt(45);
    
}

function addBuoy(location,scale_factor){
  const svgMarker = {
    path: `M8 12H12M12 12H16M12 12V16M12 12V8M12 21C7.02944 21 3 16.9706 3 12C3 7.02944 7.02944 3 12 3C16.9706 3 21 7.02944 21 12C21 16.9706 16.9706 21 12 21Z`,
    fillColor: "blue",
    fillOpacity: 1.0,
    strokeWeight: 1,
    rotation: 0,
    scale: 1+scale_factor,
    anchor: new google.maps.Point(0, 20),
  }
const marker = new google.maps.Marker({
  icon: svgMarker,
  position: location,
  map: map,
});

}

window.initMap = initMap();
get(child(dbRef,"/cluster/")).then((snapshot)=> {
  const data=snapshot.val();
  const lat=data["centroid lat"];
  const lng=data["centroid long"];
  const ppc=data["ppc"];
  const cp=ppc;
  const m = Math.max(...cp);
  for(let i in ppc)
  {
    cp[i]=parseFloat(ppc[i])/m;
  }
  for(let i in lat)
  {
    var centerPoint = new google.maps.LatLng(lat[i],lng[i]);
    addBuoy(centerPoint,ppc[i]);
  }
  
})