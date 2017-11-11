#!/bin/bash -e

/run.sh "$@" &

function addsource {
  curl admin:admin@localhost:3000/api/datasources \
    -H "Content-Type: application/json" \
    --data-binary '{"name":"influxdb","type":"influxdb","access":"proxy","url":"http://influx:8086","isDefault":true,"database":"home"}'
}

until addsource; do
    echo .
    sleep 1
done
wait
