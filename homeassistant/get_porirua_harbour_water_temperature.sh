#!/usr/bin/env bash
#curl -s "http://hilltop.gw.govt.nz/Data.hts?Service=Hilltop&Request=GetData&Site=Porirua%20Harbour%20at%20Mana%20Cruising%20Club&Measurement=Water%20Temperature&From=$(date --date "30 minutes ago" +"%d/%m/%y%%20%H:%M:%S")&To=$(date +"%d/%m/%y%%20%H:%M:%S")&interval=" | xmllint --xpath "//Hilltop/Measurement/Data/E[last()]/I1/text()" -
curl -s "http://hilltop.gw.govt.nz/Data.hts?Service=Hilltop&Request=GetData&Site=Porirua%20Harbour%20at%20Mana%20Cruising%20Club&Measurement=Water%20Temperature" | xmllint --xpath "//Hilltop/Measurement/Data/E[last()]/I1/text()" -
