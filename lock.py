import ssl
from urllib import request, parse
from urllib3.util import SSLContext
import json


class Lock:
    def __init__(self, lock_id, name, state, url, key):
        self.lock_id = lock_id
        self.name = name
        self.state = state
        self.url = url
        self.key = key

    # Call lock to close
    def lock(self):
        self.state = 0
        self.make_authenticated_request("/lock")

    # Call lock to open
    def unlock(self):
        self.state = 1
        self.make_authenticated_request("/unlock")

    # Get current state of the lock
    def get_state(self):
        state = self.make_authenticated_request("/state")
        self.state = str(state)

    # Makes request to the lock with the key passed
    def make_authenticated_request(self, endpoint):
        # Encode auth key
        data_obj = {
            'auth': self.key
        }
        data = parse.urlencode(data_obj).encode()

        # Allow self-signed certificate
        context = SSLContext()
        context.verify_mode = ssl.CERT_NONE

        # Make request
        print("Lock request: " + self.url + endpoint)
        req = request.Request(self.url + endpoint, data=data)
        response = request.urlopen(req, context=context)
        return response.read().decode()
