import cv2
import numpy as np

def write_frame(last_frame, frame, file):
    for i in range(rows):
        for j in range(cols):
            k = frame[i,j]
            #print(k)
            #file.write("PUSH 255\n")
            #file.write("PUSH 256\n")
            #file.write("MUL\n")
            #file.write(f"PUSH {k[0]}\n")
            #file.write("ADD\n")
            #file.write("PUSH 256\n")
            #file.write("MUL\n")
            #file.write(f"PUSH {k[1]}\n")
            #file.write("ADD\n")
            #file.write("PUSH 256\n")
            #file.write("MUL\n")
            #file.write(f"PUSH {k[2]}\n")
            #file.write("ADD\n")
            if last_frame is None or not np.array_equal(frame[i,j], last_frame[i,j]):
                file.write(f"push {70 if ((int(k[0]) + int(k[1]) + int(k[2])) > 390) else 0}\n")
                file.write(f"push {100*i+j}\n")
                file.write("popr rax\n")
                file.write("popv [rax]\n")
    file.write("drw\n")
    #file.write("HLT\n")

file = open("output.asm", "w")
cap = cv2.VideoCapture("badapple_lowres.mp4")
rows = 100
cols = 100

if not cap.isOpened():
    print("Error: Could not open video file.")
else:
    print("Video file opened successfully!")

ret, frame = cap.read()
last_frame = frame

write_frame(None, frame, file)
cv2.imshow("video", frame)

i = 0
while ret:
    write_frame(last_frame, frame, file)
    i+=1
    print("frame", i)
    cv2.imshow("video", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'): break
    last_frame = frame
    ret, frame = cap.read()

file.write("pause\nhlt\n")
cap.release()
file.close()
