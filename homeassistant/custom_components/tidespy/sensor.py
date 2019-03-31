import logging
from datetime import timedelta, datetime

import requests
import voluptuous as vol

from homeassistant.components.sensor import PLATFORM_SCHEMA
from homeassistant.const import CONF_NAME, ATTR_ATTRIBUTION
from homeassistant.helpers.entity import Entity
import pdb
import homeassistant.helpers.config_validation as cv

_LOGGER = logging.getLogger(__name__)

CONF_ATTRIBUTION = "Data provided by tidespy.com"
CONF_PLACE_ID='place_id'
CONF_API_KEY='api_key'
ICON = 'mdi:pool'
TIDESPY_URL = "https://tidespy.com/api/tideturns?pn={place_id}&key={api_key}&unit=m&start={start_date}&days=2"


PLATFORM_SCHEMA = PLATFORM_SCHEMA.extend({
    vol.Required(CONF_PLACE_ID): cv.string,
    vol.Required(CONF_API_KEY): cv.string,
})


def setup_platform(hass, config, add_devices, discovery_info=None):
    """Get the Tidespy sensor."""
    place_id = config.get(CONF_PLACE_ID)
    api_key  = config.get(CONF_API_KEY)
    add_devices([
      TidespySensor(place_id, api_key, 'High'),
      TidespySensor(place_id, api_key, 'Low')
    ])


class TidespySensor(Entity):
    def __init__(self, place_id, api_key, highOrLow):
        """Initialize the sensor."""
        self.place_id = place_id
        self.api_key = api_key
        self.highOrLow = highOrLow
        self.highOrLowCode = self.highOrLow[0]
        self.entity_id = "sensor.tidespy_{}_{}".format(self.place_id, self.highOrLow.lower())
        self._name = 'tides_{place_id}_{highOrLow}'.format(
            place_id=place_id, 
            highOrLow=highOrLow
        )
        self.update()

    @property
    def name(self):
        """Return the name of the sensor."""
        return "{} Tide for {}".format(self.highOrLow, self.tide_info.place_name)

    @property
    def state(self):
        """Return the state of the sensor."""
        return self.tide_info.next_tide(self.highOrLowCode)

    @property
    def device_state_attributes(self):
        """Return the state attributes."""
        attributes = { 
            'placeName': self.tide_info.place_name
        }
        return attributes

    @property
    def icon(self):
        """Icon to use in the frontend, if any."""
        return ICON

    def update(self):
        """Get the latest data from Tidespy and update the states."""
        url = TIDESPY_URL.format(
            api_key=self.api_key,
            place_id=self.place_id,
            start_date=datetime.now().strftime("%Y%m%d")
        )
        r = requests.get(url)
        self.tide_info = TidePlace(r.json())

class TidePlace(object):
  """ A location that we have tide turns for """

  def __init__(self, data):
    self._data = data

  @property
  def place_name(self):
    return self._data.get("Name")

  def next_tide(self, highOrLow):
    for turn in self.upcoming_turns():
      if turn.get("HorL") == str(highOrLow):
        return self.turn_timestamp(turn)

  def turn_timestamp(self, turn):
    return datetime.strptime(turn["Date"], "%Y%m%d") + timedelta(minutes=turn["Minute"])

  def upcoming_turns(self):
    for turn in self._data.get("Turns"):
      if self.turn_timestamp(turn) > datetime.now():
        yield turn
