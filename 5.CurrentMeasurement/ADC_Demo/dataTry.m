clc;
close all;
clear all;
sig=[];
x=1;
fileID = fopen('33Hz.txt');
dataFromfile = textscan(fileID, '%s');% %s for reading string values (hexadecimal numbers)
a=cell2mat(dataFromfile{1,1});
i=1;
for j=1:length(a)/4
  b = hex2dec(a(i,[1:2])); 
  i=i+1;
  c = hex2dec(a(i,[1:2]));
  i=i+1;
  d = hex2dec(a(i,[1:2])); 
  i=i+1;
  e = hex2dec(a(i,[1:2])); 
  i=i+1;
  sig(x)=swapbytes(typecast(uint8([e d c b]),'single'));
  x=x+1;
end
save sig.mat sig
%swapbytes(typecast(uint8([0x41 0xB0 0x00 0x00]),'single'))
fclose(fileID);
stepSize=3/255;
sig=sig*stepSize;
%sig=sig-mean(sig);
figure(1)
plot(sig);
xlabel('Time(sec)');
ylabel('amplitude(V)');
title('50 Hz Current Signature of a Lamp Load Fs=2048Hz');
sig_I=sig-1.5;
%AC Signal
figure(2)
plot(sig_I);
xlabel('Time(sec)');
ylabel('amplitude(V)');
title('Input Signal');
N=length(sig_I);
rmsvalue = rms(sig_I)
rmsvalue_1=sqrt((sum(abs(sig_I).^2))/N)    



fs=2048;
N=length(sig);
n=[0:N-1];

% find the spectrum of x2

Y = fft(sig);
P2 = abs(Y)

nfft=2^nextpow2(N);
freq2=fft(sig,nfft)/N;
f=fs/2*linspace(0,1,nfft/2);
figure(2)
plot(f,2*abs(freq2(1:nfft/2)));


xlabel('Frequency(Hz)')
ylabel('amplitude');
title('spectrum of the the Input signal(200 Hz and 3500 Hz)');


