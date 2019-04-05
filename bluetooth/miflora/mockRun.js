const run = function() {
  return new Promise(resolve => setTimeout(resolve, 10000));
}

module.exports = run;
