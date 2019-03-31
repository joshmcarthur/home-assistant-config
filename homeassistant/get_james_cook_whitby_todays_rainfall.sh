#!/usr/bin/env bash
curl -s "http://hilltop.gw.govt.nz/Data.hts?Service=Hilltop&Request=GetData&Site=Duck%20Creek%20at%20James%20Cook%20Reservoir&Measurement=Rainfall&From=$(date +"%d/%m/%y%%2000:00:00")&To=$(date +"%d/%m/%y%%2023:59:59")&interval=1%20Day&Alignment=1%20Day&tstype=stdqualseries" | xmllint --xpath "//Hilltop/Measurement/Data/E[last()]/I1/text()" -
