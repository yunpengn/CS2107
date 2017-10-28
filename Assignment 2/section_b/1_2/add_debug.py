#!/usr/bin/python

import sys
import json
import random
import os

def write(data, sep="\n"):
  sys.stdout.write(data + sep)
  sys.stdout.flush()

def generate_profile(name):
  strength = random.randint(1, 100)
  intelligence = random.randint(1, 100)
  agility = random.randint(1, 100)
  speed = random.randint(1, 50)
  level = random.randint(1, 10)
  debug = ['flag']
  uid = os.urandom(20).encode("hex")
  profile_object = {'name': name,
                    'strength': strength,
                    'intelligence': intelligence,
                    'agility': agility,
                    'speed': speed,
                    'level': level,
                    'uid': uid,
                    'debug': debug}
  return profile_object

profile = generate_profile("Neil")
write("Your generated profile data: ")
data = json.dumps(profile).encode('base64').replace("\n", "")
write(data)