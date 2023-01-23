import RPi.GPIO as GPIO
switch_pin = 29
nb_pin = 31
GPIO.setmode(GPIO.BOARD)
GPIO.setup(switch_pin, GPIO.OUT)
GPIO.setup(nb_pin, GPIO.OUT)
GPIO.setup(switch_pin,0)
GPIO.setup(nb_pin,0)



import face_recognition
#import cv2
import numpy as np
import picamera

from time import sleep

from subprocess import call

"""
CMake needed
pip install face-recognition

"""

code = [1,0,1,1,0,1,1,0,1,1]
timer = 1



def givenb(num):
    global timer
    GPIO.output(nb_pin,num)
    GPIO.output(switch_pin,timer)
    if(timer==0):timer=1
    else:timer=0
    sleep(0.05)



# https://github.com/ageitgey/face_recognition/blob/master/examples/facerec_on_raspberry_pi.py
# Sur le raspberry pi, il faut utilisr camera = picamera.PiCamera()
camera = picamera.PiCamera()
camera.resolution = (320, 240)
output = np.empty((240, 320, 3), dtype=np.uint8)

# à ajouter la photo de la personne à identifier
person_image = face_recognition.load_image_file("/home/pi/projet1/photos_train/matthis.jpg")
person_face_encoding = face_recognition.face_encodings(person_image)[0]

# Create arrays of known face encodings and their names
known_face_encodings = [person_face_encoding]
known_face_names = ["Person"]

face_locations = []
face_encodings = []
face_names = []
process_this_frame = True


cond = True
while cond:
    camera.capture(output, format="rgb")
    # Find all the faces and face encodings in the current frame of video
    face_locations = face_recognition.face_locations(output)
    face_encodings = face_recognition.face_encodings(output, face_locations)
    face_names = []
    for face_encoding in face_encodings:
            # See if the face is a match for the known face(s)
            matches = face_recognition.compare_faces(known_face_encodings,
                                                     face_encoding)
            name = "Unknown"

            # Or instead, use the known face with the smallest distance to the new face
            face_distances = face_recognition.face_distance(
                known_face_encodings, face_encoding)
            best_match_index = np.argmin(face_distances)
            if matches[best_match_index]:
                name = known_face_names[best_match_index]

            face_names.append(name)

    process_this_frame = not process_this_frame

    for (top, right, bottom, left), name in zip(face_locations, face_names):
        # Scale back up face locations since the frame we detected in was scaled to 1/4 size
        print("name" + name)
        cond=False
        for i in code:
            givenb(i)
        GPIO.output(switch_pin,0)
        GPIO.cleanup()
        # Draw a box around the face
        #cv2.rectangle(frame, (left, top), (right, bottom), (0, 0, 255), 2)

        # Draw a label with a name below the face
        #cv2.rectangle(frame, (left, bottom - 35), (right, bottom), (0, 0, 255),
        #              cv2.FILLED)
        #font = cv2.FONT_HERSHEY_DUPLEX
        #cv2.putText(frame, name, (left + 6, bottom - 6), font, 1.0,
        #            (255, 255, 255), 1)

    # Display the resulting image
    #cv2.imshow('Video', frame)

    # Hit 'q' on the keyboard to quit!
    #if cv2.waitKey(1) & 0xFF == ord('q'):
    #    break

call("sudo  poweroff", shell=True)
# Release handle to the webcam
#camera.release()
#cv2.destroyAllWindows()
