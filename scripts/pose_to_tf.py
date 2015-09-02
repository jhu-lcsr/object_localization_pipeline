#!/usr/bin/env python

import rospy
import tf
from tf_conversions import posemath as pm

def link_cb(msg):
    pass

def node_cb(msg):
    pass

if __name__ == '__main__':
    rospy.init_node('assembly_node_link_to_tf')

    sub1 = rospy.Subscriber('links',link_cb)
    sub2 = rospy.Subscriber('nodes',node_cb)
