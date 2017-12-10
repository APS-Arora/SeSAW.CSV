#!/usr/bin/env python

import roslib; roslib.load_manifest('keyboard_control')

from Tkinter import *
import rospy
from std_msgs.msg import String
from sesaw_msgs.msg import ThrusterCommand
from sesaw_msgs.srv import CollectCommand

rospy.init_node('keyboard_control_vehicle_node', anonymous=True)

root = Tk()

frame = Frame(root, width=100, height=100)

label_vars = [StringVar(),StringVar()]
label_vars[0].set('empty')
label_vars[1].set('empty')

def editGui(data):

    global FIRST_ITERATION
    global previous6
    global present6

    label_vars[0].set(data.leftThrust)
    label_vars[1].set(data.rightThrust)

    present6.leftThrust = previous6.leftThrust = data.leftThrust
    present6.rightThrust = previous6.rightThrust = data.rightThrust

# Keyboard Control

# w - forward
# a - left
# s - backward
# d - right

string_dict = {0 : "left", 1 : "right", 2 : "backward", 3 : "forward", 4 : "STOP", 5 : "collect"}

def create_callbacks(arg):

    def callback(ev=None):

        '''
        the argument consists the thruster number whose value has to be changed.

        global variable previous6 has the current values of thrusters.

        We will have to put the new value into present6 and
        then return from this function. This will publish the value in present6
        instead of previous6, thus changing the values given to the thrusters.
        '''

        print "Entered callback with thruster value ", arg

        global present6
        global previous6
        global THRUSTER_VALUES_CHANGED
        global pub6

        present6.leftThrust = previous6.leftThrust
        present6.rightThrust = previous6.rightThrust

        if arg == 0:

            present6.leftThrust = previous6.leftThrust - MIN_THRUST_INPUT
            present6.rightThrust = previous6.rightThrust + MIN_THRUST_INPUT

        if arg == 1:

            present6.leftThrust = previous6.leftThrust + MIN_THRUST_INPUT
            present6.rightThrust = previous6.rightThrust - MIN_THRUST_INPUT

        if arg == 2:

            present6.leftThrust = previous6.leftThrust - MIN_THRUST_INPUT
            present6.rightThrust = previous6.rightThrust - MIN_THRUST_INPUT

        if arg == 3:

            present6.leftThrust = previous6.leftThrust + MIN_THRUST_INPUT
            present6.rightThrust = previous6.rightThrust + MIN_THRUST_INPUT


        if arg == 4:

            present6.leftThrust = 0.
            present6.rightThrust = 0.

        if arg == 5:

            try:
                print "Trying to collect garbage."
                collectGarbage()
            except rospy.ServiceException, e:
                print "Garbage Collection Service call failed: %s"%e
            else:
                print "Garbage Collected Successfully"
        else:
            THRUSTER_VALUES_CHANGED = True

            print "Previous values: "
            print previous6

            print "Present values: "
            print present6

            pub6.publish(present6)

    return callback

left = Button(frame, text="left(A)", command=create_callbacks(0))
left.grid(row=1,column=0)

back = Button(frame, text="backward (S)", command=create_callbacks(2))
back.grid(row=1,column=1)

right = Button(frame, text="right (D)", command=create_callbacks(1))
right.grid(row=1,column=2)

forward = Button(frame, text="forward (W)", command=create_callbacks(3))
forward.grid(row=0,column=1)

stop = Button(frame, text="STOP (Space)", bg='red', command=create_callbacks(4))
stop.grid(row=1,column=3)

collect = Button(frame, text="Collect (C)", bg='green', command=create_callbacks(5))
collect.grid(row=2,column=3)

l1 = Label(frame, text="Force Values")
l1.grid(row=0, column=4)

for i in range(len(label_vars)):
    temp = Label(frame, textvariable=label_vars[i])
    temp.grid(row=i+1, column=4)

root.bind("a", create_callbacks(0))
root.bind("d", create_callbacks(1))
root.bind("s", create_callbacks(2))
root.bind("w", create_callbacks(3))
root.bind("p",create_callbacks(4))
root.bind("<space>", create_callbacks(4))
root.bind("c",create_callbacks(5))

frame.pack()

# rospy.init_node('keyboard_control_vehicle_node', anonymous=True)
collectGarbage = rospy.ServiceProxy('/sesaw/actuators/Collect', CollectCommand)
rospy.Subscriber("/sesaw/control/Thrust", ThrusterCommand, editGui)
pub6 = rospy.Publisher("/sesaw/control/Thrust", ThrusterCommand, queue_size = 10)

present6 = ThrusterCommand()
previous6 = ThrusterCommand()
THRUSTER_VALUES_CHANGED = False
FIRST_ITERATION = True
MIN_THRUST_INPUT = 10.0
prev_time = 0.

root.mainloop()
