% animates the plotting of x and y position (hopefully)

length = numel(x);

figure(1)
hold on
for i = 1:length
    plot(x(i), y(i), '*k')
    drawnow
    hold on
end
