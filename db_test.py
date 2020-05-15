from database import database
from camera import Camera


def main():
    camera_id = 1
    print("Connecting to database...")

    db = database()
    db.select_all_locations()
    # Need to pass in camera id
    camera = db.get_camera(camera_id)
    camera.snapshot()


if __name__ == '__main__':
    main()
