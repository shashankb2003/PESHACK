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

function addBuoy(location){
  const svgMarker = {
    path: `M11 13C11 13.5523 11.4477 14 12 14C12.5523 14 13 13.5523 13 13V8C13 7.44772 12.5523 7 12 7C11.4477 7 11 7.44772 11 8V13ZM13 15.9888C13 15.4365 12.5523 14.9888 12 14.9888C11.4477 14.9888 11 15.4365 11 15.9888V16C11 16.5523 11.4477 17 12 17C12.5523 17 13 16.5523 13 16V15.9888ZM7.25007 2.38782C8.54878 2.0992 10.1243 2 12 2C13.8757 2 15.4512 2.0992 16.7499 2.38782C18.06 2.67897 19.1488 3.176 19.9864 4.01358C20.824 4.85116 21.321 5.94002 21.6122 7.25007C21.9008 8.54878 22 10.1243 22 12C22 13.8757 21.9008 15.4512 21.6122 16.7499C21.321 18.06 20.824 19.1488 19.9864 19.9864C19.1488 20.824 18.06 21.321 16.7499 21.6122C15.4512 21.9008 13.8757 22 12 22C10.1243 22 8.54878 21.9008 7.25007 21.6122C5.94002 21.321 4.85116 20.824 4.01358 19.9864C3.176 19.1488 2.67897 18.06 2.38782 16.7499C2.0992 15.4512 2 13.8757 2 12C2 10.1243 2.0992 8.54878 2.38782 7.25007C2.67897 5.94002 3.176 4.85116 4.01358 4.01358C4.85116 3.176 5.94002 2.67897 7.25007 2.38782Z`,
    fillColor: "#ff0000",
    fillOpacity: 1.0,
    strokeWeight: 1,
    rotation: 0,
    scale: 1,
    anchor: new google.maps.Point(0, 20),
  }

const marker = new google.maps.Marker({
  icon: svgMarker,
  position: location,
  map: map,
});

}

window.initMap = initMap();
get(child(dbRef,"/users/")).then((snapshot)=> {
  
  const data=snapshot.val();
  const l=Object.values(data);
  for(let i in l)
  {
    const str=l[i].split(",");
    var centerPoint = new google.maps.LatLng(str[0],str[1]);
    addBuoy(centerPoint);
  }
  
})
