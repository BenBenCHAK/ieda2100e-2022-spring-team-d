const HongKong = {
    lat: 22.302711,
    long: 114.177216,
}
let dest = {
    lat: HongKong.lat,
    long: HongKong.long,
};

const map = L.map('map').setView([HongKong.lat, HongKong.long], 9);
const attribution = '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors';
const tileURL = 'https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png';

const tiles = L.tileLayer(tileURL, {
    attribution: attribution,
    maxZoom: 18,
    id: 'mapbox/streets-v11',
    tileSize: 512,
    zoomOffset: -1,
    accessToken: 'your.mapbox.access.token'
})
tiles.addTo(map);

let marker = L.marker([HongKong.lat, HongKong.long]);
marker.addTo(map);

function selectLocation(event) {
    let lat = event.path[1].cells[1].innerText, long = event.path[1].cells[2].innerText;
    marker.setLatLng(L.latLng(lat, long));
    map.setView(L.latLng(lat, long), 15);
    dest.lat = lat;
    dest.long = long;
}
