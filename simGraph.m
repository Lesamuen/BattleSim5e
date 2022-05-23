function simGraph(fileName)
    arr = readmatrix(fileName);
    avg = arr(1,1);
    stdev = arr(2,1);
    arr(:,1) = [];
    bar(arr(1,:),arr(2,:));
    title(fileName, strcat("Average: ", string(avg), "  Std. Dev: ", string(stdev)));
    xlabel("Damage");
    ylabel("Iterations");
    hold on;
    xline(avg,'color','r','linewidth',1);
    xline(avg+stdev,'color','k','linewidth',1);
    xline(avg-stdev,'color','k','linewidth',1);
    xline(avg+2*stdev,'color','k','linewidth',1);
    xline(avg-2*stdev,'color','k','linewidth',1);
    hold off;
end