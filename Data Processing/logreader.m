
clear;
clf;

filenum = '051';
infofile = strcat('INF', filenum, '.TXT');
datafile = strcat('LOG', filenum, '.BIN');

% map from datatype to length in bytes
dataSizes.('float') = 4;
dataSizes.('ulong') = 4;
dataSizes.('int') = 4;
dataSizes.('int32') = 4;
dataSizes.('uint8') = 1;
dataSizes.('uint16') = 2;
dataSizes.('char') = 1;





%% read from info file to get log file structure
fileID = fopen(infofile);
items = textscan(fileID,'%s','Delimiter',',','EndOfLine','\r\n');
fclose(fileID);
[ncols,~] = size(items{1});
ncols = ncols/2;
varNames = items{1}(1:ncols)';
varTypes = items{1}(ncols+1:end)';
varLengths = zeros(size(varTypes));
colLength = 256;
rowLength = 6;
for i = 1:numel(varTypes)
    varLengths(i) = dataSizes.(varTypes{i});
end


R = cell(1,numel(varNames));

%% read column-by-column from datafile
fid = fopen(datafile,'rb');
for i=1:numel(varTypes)
    %# seek to the first field of the first record
    offset = sum(varLengths(1:i-1));
    fseek(fid, offset, 'bof');
    
    % read column with specified format, skipping required number of bytes
%     colLength - varLengths(i)
    R{i} = fread(fid, Inf, ['*' varTypes{i}], colLength-varLengths(i));
    eval(strcat(varNames{i},'=','R{',num2str(i),'};'));
end

fclose(fid);

% PLOTTING INTERRUPT TESTING -- GET RID OF THIS AND MAKE IT BETTER CODE
t = 0:1:max(EnvelopeTime);
trig = zeros(max(EnvelopeTime) + 1, 1);
for i = t
    if ismember(i, EnvelopeTime)
        trig(i+1) = 1;
    end
end

plot(t*1E-6, trig);





