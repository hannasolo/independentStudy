import os
from os.path import isfile, join
import urllib.request
import shutil
from datetime import datetime


class Camera:
    def __init__(self, camera_id, name, url):
        self.camera_id = camera_id
        self.name = name
        # url for the snapshot
        self.url = url
        self.folder = "static/snapshots/" + str(self.camera_id)

        # Make folder if necessary
        if not os.path.exists(self.folder):
            os.makedirs(self.folder)

    # Take a picture and save it locally
    def snapshot(self):
        # Download the file from the cameras snapshot url:
        # ex: camera_{CAMERAID}_{YYYYMMDDHHMMSS}.jpg
        filename = self.folder + "/camera_" + str(self.camera_id) + "_" + datetime.now().strftime(
            "%Y%m%d%H%M%S") + ".jpg"
        with urllib.request.urlopen(self.url) as response, open(filename, 'wb') as out_file:
            shutil.copyfileobj(response, out_file)

    def get_captures(self):
        captures = []

        for f in os.listdir(self.folder):
            if isfile(join(self.folder, f)):
                captures.append(join(self.folder, f))

        # Sort by datestamp
        captures.sort(reverse=True)
        return captures
