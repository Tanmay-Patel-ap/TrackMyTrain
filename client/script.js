const API_BASE = '';

function switchSection(id) {
  document.querySelectorAll('.feature-section').forEach(s => s.classList.remove('active'));
  document.querySelectorAll('.nav-btn').forEach(b => b.classList.remove('active'));
  document.getElementById('section-' + id).classList.add('active');
  document.querySelector('.nav-btn[data-section="' + id + '"]').classList.add('active');
  document.getElementById('result').classList.remove('visible');
}

document.querySelectorAll('.nav-btn').forEach(btn => {
  btn.addEventListener('click', () => switchSection(btn.dataset.section));
});

function showResult(title, content, isError) {
  const el = document.getElementById('result');
  el.classList.add('visible');
  el.classList.remove('error');
  if (isError) el.classList.add('error');
  el.innerHTML = '<h3>' + title + '</h3>' + content;
}

function hideResult() {
  document.getElementById('result').classList.remove('visible');
}

async function apiGet(endpoint) {
  const resp = await fetch(API_BASE + endpoint);
  return resp.json();
}

function getVal(id) {
  return document.getElementById(id).value.trim();
}

function setResult(id, html) {
  document.getElementById('result-' + id).innerHTML = html;
}

async function getSchedule() {
  const trainNo = getVal('schedule-trainNo');
  if (!trainNo) { showResult('Error', 'Please enter a train number.', true); return; }
  const data = await apiGet('/api/train/schedule?trainNo=' + encodeURIComponent(trainNo));
  if (!data.success) { showResult('Error', data.error, true); return; }
  const d = data.data;
  let html = '<div class="detail-row"><span class="detail-label">Train</span><span class="detail-value">' + d.train.trainName + ' (' + d.train.trainNumber + ')</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Type</span><span class="detail-value">' + d.train.type + '</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Route</span><span class="detail-value">' + d.train.source + ' → ' + d.train.destination + '</span></div>';
  if (d.route && d.route.length) {
    html += '<br><table><tr><th>Station</th><th>Name</th><th>Arrival</th><th>Departure</th><th>Dist(km)</th><th>Day</th></tr>';
    d.route.forEach(function(e) {
      html += '<tr><td>' + e.stationCode + '</td><td>' + e.stationName + '</td><td>' + e.arrivalTime + '</td><td>' + e.departureTime + '</td><td>' + e.distance + '</td><td>' + e.day + '</td></tr>';
    });
    html += '</table>';
  }
  showResult('Train Schedule', html);
}

async function getPNRStatus() {
  const pnr = getVal('pnr-number');
  if (!pnr) { showResult('Error', 'Please enter a PNR number.', true); return; }
  const data = await apiGet('/api/pnr/status?pnr=' + encodeURIComponent(pnr));
  if (!data.success) { showResult('Error', data.error, true); return; }
  const d = data.data;
  let html = '<div class="detail-row"><span class="detail-label">PNR</span><span class="detail-value">' + d.pnrNumber + '</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Train</span><span class="detail-value">' + d.trainName + ' (' + d.trainNumber + ')</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Date</span><span class="detail-value">' + d.boardingDate + '</span></div>';
  html += '<div class="detail-row"><span class="detail-label">From → To</span><span class="detail-value">' + d.from + ' → ' + d.to + '</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Status</span><span class="detail-value">' + d.bookingStatus + '</span></div>';
  if (d.passengers && d.passengers.length) {
    html += '<br><table><tr><th>Name</th><th>Booking</th><th>Current</th><th>Seat</th></tr>';
    d.passengers.forEach(function(p) {
      html += '<tr><td>' + p.name + '</td><td>' + p.bookingStatus + '</td><td>' + p.currentStatus + '</td><td>' + p.seatNumber + '</td></tr>';
    });
    html += '</table>';
  }
  showResult('PNR Status', html);
}

async function getLiveStatus() {
  const trainNo = getVal('live-trainNo');
  if (!trainNo) { showResult('Error', 'Please enter a train number.', true); return; }
  const data = await apiGet('/api/train/live?trainNo=' + encodeURIComponent(trainNo));
  if (!data.success) { showResult('Error', data.error, true); return; }
  const d = data.data;
  let html = '<div class="detail-row"><span class="detail-label">Train</span><span class="detail-value">' + d.trainName + ' (' + d.trainNumber + ')</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Current Station</span><span class="detail-value">' + d.currentStation + '</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Next Station</span><span class="detail-value">' + d.nextStation + '</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Delay</span><span class="detail-value">' + d.delay + '</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Last Updated</span><span class="detail-value">' + d.lastUpdated + '</span></div>';
  showResult('Live Train Status', html);
}

async function getStationStatus() {
  const code = getVal('station-code');
  if (!code) { showResult('Error', 'Please enter a station code.', true); return; }
  const data = await apiGet('/api/station/status?code=' + encodeURIComponent(code));
  if (!data.success) { showResult('Error', data.error, true); return; }
  const d = data.data;
  let html = '<div class="detail-row"><span class="detail-label">Station</span><span class="detail-value">' + d.stationName + ' (' + d.stationCode + ')</span></div>';
  if (d.arrivingTrains && d.arrivingTrains.length) {
    html += '<br><h4>Arriving Trains</h4><table><tr><th>Number</th><th>Name</th></tr>';
    d.arrivingTrains.forEach(function(t) {
      html += '<tr><td>' + t.trainNumber + '</td><td>' + t.trainName + '</td></tr>';
    });
    html += '</table>';
  }
  if (d.departingTrains && d.departingTrains.length) {
    html += '<br><h4>Departing Trains</h4><table><tr><th>Number</th><th>Name</th></tr>';
    d.departingTrains.forEach(function(t) {
      html += '<tr><td>' + t.trainNumber + '</td><td>' + t.trainName + '</td></tr>';
    });
    html += '</table>';
  }
  if ((!d.arrivingTrains || !d.arrivingTrains.length) && (!d.departingTrains || !d.departingTrains.length)) {
    html += '<p style="color:#94a3b8;margin-top:8px;">No trains found at this station.</p>';
  }
  showResult('Station Status', html);
}

async function searchTrain() {
  const query = getVal('search-query');
  if (!query) { showResult('Error', 'Please enter a search query.', true); return; }
  const data = await apiGet('/api/train/search?query=' + encodeURIComponent(query));
  if (!data.success) { showResult('Error', data.error, true); return; }
  if (!data.data || !data.data.length) { showResult('No Results', 'No trains found.', false); return; }
  let html = '<table><tr><th>Number</th><th>Name</th><th>Source</th><th>Destination</th><th>Type</th></tr>';
  data.data.forEach(function(t) {
    html += '<tr><td>' + t.trainNumber + '</td><td>' + t.trainName + '</td><td>' + t.source + '</td><td>' + t.destination + '</td><td>' + t.type + '</td></tr>';
  });
  html += '</table>';
  showResult('Search Results (' + data.data.length + ')', html);
}

async function getTrainsBetween() {
  const from = getVal('between-from');
  const to = getVal('between-to');
  const date = getVal('between-date');
  if (!from || !to || !date) { showResult('Error', 'Please fill all fields.', true); return; }
  const data = await apiGet('/api/trains/between?from=' + encodeURIComponent(from) + '&to=' + encodeURIComponent(to) + '&date=' + encodeURIComponent(date));
  if (!data.success) { showResult('Error', data.error, true); return; }
  const d = data.data;
  let html = '<div class="detail-row"><span class="detail-label">Train</span><span class="detail-value">' + d.train.trainName + ' (' + d.train.trainNumber + ')</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Duration</span><span class="detail-value">' + d.duration + '</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Distance</span><span class="detail-value">' + d.totalDistance + ' km</span></div>';
  if (d.stations && d.stations.length) {
    html += '<br><table><tr><th>Station</th><th>Name</th><th>Arrival</th><th>Departure</th></tr>';
    d.stations.forEach(function(s) {
      html += '<tr><td>' + s.stationCode + '</td><td>' + s.stationName + '</td><td>' + s.arrivalTime + '</td><td>' + s.departureTime + '</td></tr>';
    });
    html += '</table>';
  }
  showResult('Trains Between Stations', html);
}

async function getSeatAvailability() {
  const trainNo = getVal('seat-trainNo');
  const from = getVal('seat-from');
  const to = getVal('seat-to');
  const date = getVal('seat-date');
  const cls = getVal('seat-class');
  if (!trainNo || !from || !to || !date || !cls) { showResult('Error', 'Please fill all fields.', true); return; }
  const data = await apiGet('/api/seat/availability?trainNo=' + encodeURIComponent(trainNo) + '&from=' + encodeURIComponent(from) + '&to=' + encodeURIComponent(to) + '&date=' + encodeURIComponent(date) + '&class=' + encodeURIComponent(cls));
  if (!data.success) { showResult('Error', data.error, true); return; }
  const d = data.data;
  let html = '<div class="detail-row"><span class="detail-label">Train</span><span class="detail-value">' + d.trainName + ' (' + d.trainNumber + ')</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Route</span><span class="detail-value">' + d.from + ' → ' + d.to + '</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Date</span><span class="detail-value">' + d.date + '</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Class</span><span class="detail-value">' + d.className + '</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Available Seats</span><span class="detail-value">' + d.availableSeats + '</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Fare</span><span class="detail-value">Rs. ' + d.fare + '</span></div>';
  showResult('Seat Availability', html);
}

async function getFare() {
  const trainNo = getVal('fare-trainNo');
  const from = getVal('fare-from');
  const to = getVal('fare-to');
  const cls = getVal('fare-class');
  if (!trainNo || !from || !to || !cls) { showResult('Error', 'Please fill all fields.', true); return; }
  const data = await apiGet('/api/fare?trainNo=' + encodeURIComponent(trainNo) + '&from=' + encodeURIComponent(from) + '&to=' + encodeURIComponent(to) + '&class=' + encodeURIComponent(cls));
  if (!data.success) { showResult('Error', data.error, true); return; }
  const d = data.data;
  let html = '<div class="detail-row"><span class="detail-label">Train</span><span class="detail-value">' + d.trainNumber + '</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Route</span><span class="detail-value">' + d.from + ' → ' + d.to + '</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Class</span><span class="detail-value">' + d.className + '</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Base Fare</span><span class="detail-value">Rs. ' + d.baseFare + '</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Total Fare</span><span class="detail-value">Rs. ' + d.totalFare + '</span></div>';
  html += '<div class="detail-row"><span class="detail-label">Distance</span><span class="detail-value">' + d.distance + ' km</span></div>';
  showResult('Fare Enquiry', html);
}
