import cv2
from tkinter import *
from tkinter import ttk
from threading import Thread
import time

def select_cameras():
    """
    Displays a GUI for selecting cameras and returns the list of selected camera indices.
    """
    # Global variables
    camera_previews = {}
    running = True
    selected_cameras = []

    # List available cameras
    def list_cameras():
        index = 0
        cameras = []
        while True:
            cap = cv2.VideoCapture(index)
            if not cap.read()[0]:  # Check if the camera is available
                break
            cameras.append(index)
            cap.release()
            index += 1
        return cameras

    # Function to update a specific camera preview
    def update_preview(index, label):
        cap = cv2.VideoCapture(index)
        while running:
            ret, frame = cap.read()
            if not ret:
                break
            frame = cv2.resize(frame, (320, 240))  # Resize for display

            # Encode frame to a Tk-compatible format
            img_data = cv2.imencode('.ppm', frame)[1].tobytes()

            # Schedule update to the Tkinter UI
            root.after(0, update_label, label, img_data)
            time.sleep(0.03)  # Add delay to reduce CPU usage
        cap.release()

    # Function to update the label in the main thread
    def update_label(label, img_data):
        img = PhotoImage(data=img_data)
        label.configure(image=img)
        label.image = img

    # Function to stop all previews
    def stop_previews():
        nonlocal running
        running = False
        for index, thread in camera_previews.items():
            thread["thread"].join()

    # Function to handle camera selection
    def submit_selection():
        nonlocal selected_cameras
        selected_cameras = [camera_list.get(i) for i in camera_list.curselection()]
        root.quit()

    # Start the GUI
    root = Tk()
    root.title("Camera Selection")
    frm = ttk.Frame(root, padding=10)
    frm.grid()

    # Get the available cameras
    cameras = list_cameras()

    # Create a Listbox for camera selection
    ttk.Label(frm, text="Available Cameras:").grid(column=0, row=0, sticky=W)
    camera_list = Listbox(frm, selectmode=MULTIPLE, height=6)
    for cam in cameras:
        camera_list.insert(END, f"Camera {cam}")
    camera_list.grid(column=0, row=1, sticky=(W, E))

    # Preview frames for each camera
    preview_frame = ttk.Frame(frm)
    preview_frame.grid(column=1, row=0, rowspan=2, sticky=(N, S, E, W))

    for i, cam in enumerate(cameras):
        ttk.Label(preview_frame, text=f"Camera {cam}").grid(column=0, row=i, sticky=W)
        lbl = Label(preview_frame)
        lbl.grid(column=1, row=i, sticky=W)

        # Start preview threads
        thread = Thread(target=update_preview, args=(cam, lbl), daemon=True)
        camera_previews[cam] = {"thread": thread}
        thread.start()

    # Add Submit and Quit buttons
    ttk.Button(frm, text="Submit", command=submit_selection).grid(column=0, row=2, sticky=W)
    ttk.Button(frm, text="Quit", command=root.destroy).grid(column=1, row=2, sticky=E)

    # Run the Tkinter event loop
    root.mainloop()

    # Stop all previews when GUI closes
    stop_previews()

    # Return selected cameras
    return selected_cameras

# Example usage
if __name__ == "__main__":
    selected_cameras = select_cameras()
    print("Selected cameras:", selected_cameras)

