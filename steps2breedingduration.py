#!/usr/bin/env python3
import sys

# error Handling
try:
    steps = sys.argv[1]
except IndexError:
    print("Please input a step count")
    sys.exit(0)
else:
    try:
        steps = float(steps)
    except ValueError:
        print("Please use an integer for the step count")
        sys.exit(0)

steps = float(sys.argv[1]) 
steptimeratio = float(123 / 10240)
eggtime = int(steps * steptimeratio)
totaleggs = 500 # total number of eggs to be tested
# test escalating biking duration times until the optimal value is found
# start at eggtime / 5 due to max 5 eggs in party at once
for bikingtime in range(int(eggtime / 5), eggtime): 
    # reset party before trying new biking time
    party = []
    for i in range(totaleggs):
        party.append(0)
    fail = False
    hatchcount = 0
    # ensure all eggs hatch
    while hatchcount < totaleggs:
        hatch = False
        # add times to each egg in party
        for eggnum in range(len(party)):
            if party[eggnum] > 0 and party[eggnum] <= eggtime:
                # if an egg has hatched this iteration then add reduced time
                if hatch:
                    party[eggnum] += (bikingtime - 20)
                else:
                    party[eggnum] += bikingtime
                # if an egg hatched check animation time
                if party[eggnum] >= eggtime:
                    if party[eggnum] - eggtime <= 20:
                        fail = True
                        break
                    else:
                        hatch = True
                        hatchcount += 1
            # add new egg to party then begin next iteration
            elif party[eggnum] == 0:
                if hatch:
                    party[eggnum] = (bikingtime - 20)
                else:
                    party[eggnum] = bikingtime
                break
        if fail:
            break
    if fail:
        continue
    else:
        timedurationratio = int(3500 / 83)
        # add 2 seconds for a small buffer window
        result = timedurationratio * (bikingtime + 2)
        print("Use this for the breeding duration: " + str(result))
        break
