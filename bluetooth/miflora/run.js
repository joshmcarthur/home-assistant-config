const miflora = require("miflora");
const run = async function() {
  const devices = await miflora.discover();
  return Promise.all(devices.map(d => d.querySensorValues())) 
}

module.exports = run;

