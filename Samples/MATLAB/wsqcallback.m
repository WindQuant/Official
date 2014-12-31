%本函数是用于演示用户自定义回调函数具体撰写方式
%用户可以参考
function WSQCallback(reqid,isfinished,errorid,datas,codes,fields,times,selfdata)
      if(length(codes)==0|| length(fields)==0||length(times)==0)
      else
          if(length(times)==1)
              datas = reshape(datas,length(fields),length(codes))';
          elseif (length(codes)==1)
              datas = reshape(datas,length(fields),length(times))';
          elseif(length(fields)==1)
              datas = reshape(datas,length(codes),length(times));
%               else
%                   data = mapdata{5};
          end
      end

    codestr = '';
    for( i=1:length(codes))
        codestr=strcat(codestr,codes{i},',');
    end
    
    fieldstr='';
    for i=1:length(fields)
        fieldstr=strcat(fieldstr,fields{i},',');
    end
    fprintf('%s\n%s\n',codestr,fieldstr);
    disp(datas);
    fprintf('reqid = %d\n\n',reqid);
    %disp(codes);
    %disp(fields);
end