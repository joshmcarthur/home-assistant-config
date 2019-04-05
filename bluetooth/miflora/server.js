const express = require("express");
const app = express();
const run = require("./mockRun");

const port = process.env.PORT || 3000;
const CACHE_KEY = "miflora_readings";
const CACHE_EXPIRY = 60 * 60 * 1000;

app.get("/", (req, res) => {
  let cachedValue = mcache.get(CACHE_KEY);
  if (cachedValue) {
    res.send(cachedValue);
  } else {
    run().then(value => res.send(mcache.put(CACHE_KEY, value, CACHE_EXPIRY)))
  }
});
