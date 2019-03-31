# homeassistant-config

My HomeAssistant.io configuration files. 

## How I've set this up:

1. Add secrets in `secrets.yml` (necessary secrets can be found by searching `configuration.yml` for `!secret`
2. Update `known_devices.yml` with details of whether devices should be used for presence detection or not (I do not store this in git as it contains almost exclusively sensitive information)
3. Run `docker-compose up`. Actually, I run `docker-compose up -d` so the process runs in the background.
4. Port 8123 is exposed on the host, done.


