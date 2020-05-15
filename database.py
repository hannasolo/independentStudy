import hashlib
import random
import string
from camera import Camera
import sqlite3
from sqlite3 import Error
from lock import Lock
from sensor import Sensor


class database:
    def __init__(self):
        db_file = r"BananaSec.sqlite"

        # create a database connection
        self.conn = self.create_connection(db_file)

    def create_connection(self, db_file):
        """ create a database connection to the SQLite database
            specified by the db_file
        :param db_file: database file
        :return: Connection object or None
        """
        conn = None
        try:
            conn = sqlite3.connect(db_file)
        except Error as e:
            print(e)

        return conn

    # Locations
    def select_all_locations(self):
        """
        Query all rows in the locations table
        :param conn: the Connection object
        :return:
        """
        cur = self.conn.cursor()
        cur.execute("SELECT * FROM locations")

        rows = cur.fetchall()

        for row in rows:

            print(row)

    # Cameras
    def get_cameras(self):
        cur = self.conn.cursor()
        cur.execute("SELECT camera_id, name, url FROM cameras")
        cameras = []

        rows = cur.fetchall()

        for row in rows:
            cameras.append(Camera(row[0], row[1], row[2]))

        return cameras

    def get_camera(self, camera_id):
        print("Scanning database for camera info...")
        """
        Query all rows in the cameras table
        :param conn: the Connection object
        :return:
        """
        cur = self.conn.cursor()
        # SANITIZED
        cur.execute("SELECT camera_id, name, url FROM cameras WHERE camera_id=:id", {"id": camera_id})

        rows = cur.fetchall()

        if len(rows) > 0:
            return Camera(rows[0][0], rows[0][1], rows[0][2])
        else:
            return None

    def get_cameras_by_sensor_id(self, sensor_id):
        cur = self.conn.cursor()
        # SANITIZED
        cur.execute("SELECT c.camera_id, c.name, c.url FROM location_sensor ls INNER JOIN location_camera lc ON ls.location_id = lc.location_id INNER JOIN cameras c ON lc.camera_id = c.camera_id WHERE ls.sensor_id = :id", {"id": sensor_id})
        cameras = []

        rows = cur.fetchall()

        for row in rows:
            cameras.append(Camera(row[0], row[1], row[2]))

        return cameras

    def get_cameras_by_lock_id(self, lock_id):
        cur = self.conn.cursor()
        # SANITIZED
        cur.execute("SELECT c.camera_id, c.name, c.url FROM location_lock ll INNER JOIN location_camera lc ON ll.location_id = lc.location_id INNER JOIN cameras c ON lc.camera_id = c.camera_id WHERE ll.lock_id = :id", {"id": lock_id})
        cameras = []

        rows = cur.fetchall()

        for row in rows:
            cameras.append(Camera(row[0], row[1], row[2]))

        return cameras

    # Sensors
    def get_sensors(self):
        cur = self.conn.cursor()
        cur.execute("SELECT sensor_id, name, state, pin FROM sensors")
        sensors = []

        rows = cur.fetchall()

        for row in rows:
            sensors.append(Sensor(row[0], row[1], row[2], row[3]))

        return sensors

    def update_sensor_state(self, sensor):
        cur = self.conn.cursor()
        # SANITIZED
        cur.execute("UPDATE sensors SET state = :state WHERE sensor_id = :id", {"id": sensor.sensor_id, "state": sensor.state})
        self.conn.commit()

    # Locks
    def get_locks(self, ):
        cur = self.conn.cursor()
        # SANITIZED
        cur.execute("SELECT lock_id, name, state, url, key FROM locks ORDER BY lock_id")
        locks = []

        rows = cur.fetchall()

        for row in rows:
            locks.append(Lock(row[0], row[1], row[2], row[3], row[4]))
       
        return locks
        
    def get_lock_by_id(self, lock_id):
        cur = self.conn.cursor()
        # SANITIZED
        cur.execute("SELECT lock_id, name, state, url, key FROM locks WHERE lock_id=:id", {"id": lock_id})

        rows = cur.fetchall()

        if len(rows) > 0:
            return Lock(rows[0][0], rows[0][1], rows[0][2], rows[0][3], rows[0][4])
        else:
            return None

    def update_lock_state(self, lock):
        cur = self.conn.cursor()
        # SANITIZED
        cur.execute("UPDATE locks SET state = :state WHERE lock_id = :id", {"id": lock.lock_id, "state": lock.state})
        self.conn.commit()

    # NFC
    def check_nfc_serial(self, serial_number):
        cur = self.conn.cursor()
        # SANITIZED
        cur.execute("SELECT COUNT(*) FROM nfc_tags WHERE serial_number=:serial", {"serial": serial_number})

        rows = cur.fetchall()

        if int(rows[0][0]) == 1:
            return True
        else:
            return False

    def get_setting(self, setting):
        cur = self.conn.cursor()
        # SANITIZED
        cur.execute("SELECT VALUE FROM settings WHERE setting=:setting", {"setting": setting})

        row = cur.fetchone()

        if row is not None:
            return row[0]
        else:
            return None

    def set_setting(self, setting, value):
        cur = self.conn.cursor()
        # SANITIZED
        cur.execute("UPDATE settings SET value = :value WHERE setting = :setting", {"value": value, "setting": setting})
        self.conn.commit()

    def validate_credentials(self, username, password):
        cur = self.conn.cursor()
        # SANITIZED
        cur.execute("SELECT salt FROM users WHERE username=:username", {"username": username})

        rows = cur.fetchall()

        if len(rows) > 0:
            salt = rows[0][0]
            dk = hashlib.pbkdf2_hmac('sha256', bytes(password, encoding='utf-8'), bytes(salt, encoding='utf-8'), 100000)
            hashed = dk.hex()

            cur.execute("SELECT COUNT(*) FROM users WHERE username=:username AND password=:password", {"username": username, "password": hashed})

            rows = cur.fetchall()

            if int(rows[0][0]) == 1:
                session = self.generate_session_key()
                cur.execute("UPDATE users SET session = :sess WHERE username = :username",
                            {"sess": session, "username": username})
                self.conn.commit()
                return session
            else:
                return None

        else:
            return None

    def validate_session(self, session):
        cur = self.conn.cursor()
        cur.execute("SELECT COUNT(*) FROM users WHERE session=:sess",
                    {"sess": session})

        rows = cur.fetchall()

        if int(rows[0][0]) == 1:
            return True
        else:
            return False

    def generate_session_key(self):
        letters_and_digits = string.ascii_letters + string.digits
        return ''.join((random.choice(letters_and_digits) for i in range(32)))
