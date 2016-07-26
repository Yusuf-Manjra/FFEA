import sys


# This test only works for single threaded runs. On multi-threaded runs, threads consume a 
#   different amount of random numbers every time, and so this direct comparison will fail. 

# Still, it is important to use checkpoints, in order to have uncorrelated streams, as 
#   otherwise artifacts could rise. 
# 
# sphere_63_120_nomass_6-10steps.cpt must be the same as sphere_63_120_nomass_10steps.cpt. Exactly.
CPT = ["sphere_63_120_nomass_6-10steps.cpt", "sphere_63_120_nomass_10steps.cpt"]
STA = []
for f in CPT:
  sta = open(f,'r')
  STA.append(sta.readlines())
  sta.close()

for i in range(2):
  if STA[0][i] != STA[1][i]:
    print "checkpoint files: ", CPT[0], " and ", CPT[1], " should be exactly the same"
    sys.exit(1)

# measurements can be SLIGHTLY different, because the trajectory positions that will be used for restarts
#  were not saved with infinite accuracy.
MSM = ["sphere_63_120_nomass_I_measurement.out", "sphere_63_120_nomass_II_measurement.out"]
STA = []
for f in MSM:
  tmpT = []
  with open(f, 'r') as sta:
    while (sta.readline() != "Measurements:\n"):
      continue
    sta.readline() # skip the header
    for line in sta:
      if line.count("RESTART"): continue  
      tmpB = []
      for m in line.split():
        tmpB.append(float(m))
    tmpT.append(tmpB)
  STA.append(tmpT)


Dmax = 0.0e0
Dmin = 100.0e0
for i in range(len(STA[0])):
  for j in range(len(STA[0][i])):
    d = abs((STA[1][i][j] - STA[0][i][j])/STA[0][i][j])
    if d < Dmin: Dmin = d
    if d > Dmax: Dmax = d 
  
print "DMax: ", Dmax
print "Dmin: ", Dmin

if (Dmax > 5e-4): # the max error I got using Intel and GCC and different compiler flags was < 2e-4.
  sys.exit(1)
else:
  sys.exit(0)


