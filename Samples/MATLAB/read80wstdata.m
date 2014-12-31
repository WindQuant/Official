function read80WSTdata(w)
    if nargin<1
        %建立Wind对象；
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            w=windmatlab;
        else
            w=gWindData;
        end
    end

    strnameAll='000300.SH,IF1210.CFE,IF1211.CFE,IF1212.CFE,IF1303.CFE,000001.SZ,000002.SZ,000024.SZ,000039.SZ,000063.SZ,000100.SZ,000401.SZ,000425.SZ,000527.SZ,000538.SZ,000568.SZ,000629.SZ,000651.SZ,000686.SZ,000725.SZ,000858.SZ,000878.SZ,000895.SZ,002069.SZ,002142.SZ,002378.SZ,002570.SZ,002594.SZ,600000.SH,600015.SH,600028.SH,600029.SH,600030.SH,600031.SH,600123.SH,600125.SH,600150.SH,600166.SH,600169.SH,600177.SH,600183.SH,600188.SH,600256.SH,600362.SH,600415.SH,600489.SH,600497.SH,600500.SH,600519.SH,600528.SH,600535.SH,600547.SH,600600.SH,600690.SH,600718.SH,600795.SH,600804.SH,600837.SH,600859.SH,600875.SH,600895.SH,600999.SH,601009.SH,601088.SH,601099.SH,601106.SH,601117.SH,601168.SH,601186.SH,601328.SH,601333.SH,601600.SH,601601.SH,601628.SH,601668.SH,601808.SH,601857.SH,601866.SH,601899.SH,601919.SH,601989.SH,601991.SH,601998.SH'
    %建立Wind对象；
    answer=who('w');
    if(isempty(answer) || ~isa(w,'windmatlab'))
        w= windmatlab;
    end

    name300=regexp(strnameAll,'[,]','split');

    %开始与结束时间
    begintime=datestr(now,'yyyymmdd 09:15:00');
    endtime  =datestr(now,'yyyymmdd 15:00:00');

    argfields='asize1,asize2,asize3,asize4,asize5,ask1,ask2,ask3,ask4,ask5,bsize1,bsize2,bsize3,bsize4,bsize5,bid1,bid2,bid3,bid4,bid5,volume,amt,pre_oi,oi,last,ask,bid';

    begini=1,endi=length(name300);
    for i =begini:endi
        code = name300{i};
        aa=regexp(code,'[.]','split');
        fprintf('reading %d %s\n ',i,code);

        [data,codes,fields,times,errorid,id] =w.wst(code,argfields,begintime,endtime);
        save(strcat(aa{1},aa{2},datestr(now,'yyyymmdd'),'WST'),'data','codes','fields','times');
        [n,m] =size(data);
        fprintf('save %d %s size(data)=%d*%d\n',i,code,n,m);
    end
end
