openLoop = [0 0 0 0 0 150 50 0 0 50 0 0 0 0 0 0 0 0 50 50];
hybridLoop = [100 100 100 100 150 100 100 100 100 150 150 150 100 100 150 100 100 150 100 150];
closedLoop = [100 100 150 150 100 150 150 150 0 100 100 90 100 100 150 150 150 100 100 150];
[h1,p1] = ttest(hybridLoop,openLoop);
[h2,p2] = ttest(openLoop,closedLoop);
[h3,p3] = ttest(closedLoop,hybridLoop);

M1 = mean(openLoop)
M2 = mean(hybridLoop)
M3 = mean(closedLoop)
S1 = std(openLoop)
S2 = std(hybridLoop)
S3 = std(closedLoop)
data = [M1 S1; M2 S2; M3 S3];
bar(data);
ylim([0 150]);
xticks(1:3);
xticklabels({'openLoop','hybridLoop','closedLoop'});
ylabel('Scores');
xlabel('Control Methods');
aa = {'Average', 'Standard Deviation'};
legend(aa,'Location','Northeast');

