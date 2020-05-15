from flask import Flask, jsonify, request, render_template, redirect
from database import database
import threading
import time

app = Flask(__name__)


# Runs in a separate thread for sensor polling
def start_sensor_checks():
    db = database()
    sensors = db.get_sensors()

    armed = db.get_setting("armed")

    while armed == "1":
        for sensor in sensors:
            # Check GPIO for sensor state
            sensor.update_state()

            # Update state in DB
            db.update_sensor_state(sensor)

            # Trip if lock is opened
            if sensor.state == 1:
                cameras = db.get_cameras_by_sensor_id(sensor.sensor_id)

                # Take pictures on all available cameras that cover the area
                for camera in cameras:
                    camera.snapshot()

                # Send notifications
        # Wait a bit for next poll
        time.sleep(10)

        armed = db.get_setting("armed")


@app.route('/nfc_unlock')
def nfc_unlock():
    db = database()
    serial_number = request.args.get('serial_number')
    lock_id = request.args.get('lock_id')
    lock = db.get_lock_by_id(lock_id)

    # Take pictures from all cameras in the area
    cameras = db.get_cameras_by_lock_id(lock_id)

    for camera in cameras:
        camera.snapshot()

    if db.check_nfc_serial(serial_number):
        if lock is not None:
            # Get lock state from lock web call
            lock.get_state()
            # Save lock state
            db.update_lock_state(lock)

            if lock.state == 1:
                lock.lock()
            return "1"
        else:
            return "0"
    else:
        return "0"


@app.route('/')
def home():
    return render_template('index.html')


@app.route('/login', methods=['POST'])
def login():
    username = request.form.get('username')
    password = request.form.get('password')

    db = database()

    session = db.validate_credentials(username, password)

    if session is not None:
        return redirect("/dashboard?sid=" + session, code=302)
    else:
        return redirect("/failed", code=302)


@app.route('/dashboard')
def dashboard():
    sid = request.args.get('sid')

    db = database()

    if db.validate_session(sid):
        return render_template('dashboard.html')
    else:
        return redirect("/failed", code=302)


@app.route('/failed')
def failed():
    return render_template('loginfailed.html')


@app.route('/captures')
def captures():
    sid = request.args.get('sid')

    db = database()

    if db.validate_session(sid):
        return render_template('captures.html')
    else:
        return redirect("/failed", code=302)


# AJAX ENDPOINTS
@app.route('/get_dashboard_data', methods=['POST'])
def get_dashboard_data():
    sid = request.form.get('sid')

    db = database()

    if db.validate_session(sid):
        # Locks
        locks = db.get_locks()
        json_locks = []
        for lock in locks:
            json_locks.append(lock.__dict__)

        # Cameras
        cameras = db.get_cameras()
        json_cameras = []
        for camera in cameras:
            json_cameras.append(camera.__dict__)

        # Sensors
        sensors = db.get_sensors()
        json_sensors = []
        for sensor in sensors:
            json_sensors.append(sensor.__dict__)

        # Armed
        armed = db.get_setting("armed")

        return jsonify({
            "error": False,
            "message": "",
            "armed": armed,
            "cameras": json_cameras,
            "locks": json_locks,
            "sensors": json_sensors
        })
    else:
        return jsonify({
            "error": True,
            "message": "sid expired or invalid"
        })


@app.route('/get_captures', methods=['POST'])
def get_captures():
    sid = request.form.get('sid')
    camera_id = request.form.get('camera_id')

    db = database()

    if db.validate_session(sid):
        camera = db.get_camera(camera_id)

        return jsonify({
            "error": False,
            "message": "",
            "camera": camera.__dict__,
            "captures": camera.get_captures()
        })
    else:
        return jsonify({
            "error": True,
            "message": "sid expired or invalid"
        })


@app.route('/get_locks', methods=['POST'])
def get_locks():
    sid = request.form.get('sid')

    db = database()

    if db.validate_session(sid):
        locks = db.get_locks()

        json_locks = []
        for lock in locks:
            json_locks.append(lock.__dict__)

        return jsonify({
            "error": False,
            "message": "",
            "locks": json_locks
        })
    else:
        return jsonify({
            "error": True,
            "message": "sid expired or invalid"
        })


@app.route('/get_cameras', methods=['POST'])
def get_cameras():
    sid = request.form.get('sid')

    db = database()

    if db.validate_session(sid):
        cameras = db.get_cameras()

        json_cameras = []
        for camera in cameras:
            json_cameras.append(camera.__dict__)

        return jsonify({
            "error": False,
            "message": "",
            "cameras": json_cameras
        })
    else:
        return jsonify({
            "error": True,
            "message": "sid expired or invalid"
        })


@app.route('/get_sensors', methods=['POST'])
def get_sensors():
    sid = request.form.get('sid')

    db = database()

    if db.validate_session(sid):
        sensors = db.get_sensors()

        json_sensors = []
        for sensor in sensors:
            json_sensors.append(sensor.__dict__)

        return jsonify({
            "error": False,
            "message": "",
            "cameras": json_sensors
        })
    else:
        return jsonify({
            "error": True,
            "message": "sid expired or invalid"
        })


@app.route('/unlock', methods=['POST'])
def unlock():
    sid = request.form.get('sid')
    lock_id = request.form.get('lock_id')

    db = database()

    if db.validate_session(sid):
        lock = db.get_lock_by_id(lock_id)

        if lock is not None:
            # Send request to lock
            lock.unlock()

            return jsonify({
                "error": False,
                "message": lock.name + " lock opened"
            })
        else:
            return jsonify({
                "error": True,
                "message": "Lock not found"
            })
    else:
        return jsonify({
            "error": True,
            "message": "sid expired or invalid"
        })


@app.route('/lock', methods=['POST'])
def lock():
    sid = request.form.get('sid')
    lock_id = request.form.get('lock_id')

    db = database()

    if db.validate_session(sid):
        lock = db.get_lock_by_id(lock_id)

        if lock is not None:
            # Send request to lock
            lock.lock()

            return jsonify({
                "error": False,
                "message": lock.name + " lock closed"
            })
        else:
            return jsonify({
                "error": True,
                "message": "Lock not found"
            })


@app.route('/arm', methods=['POST'])
def arm():
    sid = request.form.get('sid')

    db = database()

    if db.validate_session(sid):
        db.set_setting("armed", "1")
        # Start polling sensors
        sensor_thread = threading.Thread(target=start_sensor_checks)
        sensor_thread.start()

        return jsonify({
            "error": False,
            "message": "System armed!"
        })
    else:
        return jsonify({
            "error": True,
            "message": "sid expired or invalid"
        })


@app.route('/disarm', methods=['POST'])
def disarm():
    sid = request.form.get('sid')

    db = database()

    if db.validate_session(sid):
        db.set_setting("armed", "0")

        return jsonify({
            "error": False,
            "message": "System disarmed!"
        })
    else:
        return jsonify({
            "error": True,
            "message": "Invalid authentication"
        })

# END AJAX ENDPOINTS


if __name__ == '__main__':
    # Start the server
    app.run(host='0.0.0.0', port=443, ssl_context=('cert.pem', 'key.pem'))
