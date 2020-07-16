sopenLoop = [0 0 0 0 0 150 50 0 0 50 0 0 0 0 0 0 0 0 50 50];
hybridLoop = [100 100 100 100 150 100 100 100 100 150 150 150 100 100 150 100 100 150 100 150];
closedLoop = [100 100 150 150 100 150 150 150 0 100 100 90 100 100 150 150 150 100 100 150];
[h1,p1] = ttest(hybridLoop,openLoop)
[h2,p2] = ttest(openLoop,closedLoop)
[h3,p3] = ttest(closedLoop,hybridLoop)







