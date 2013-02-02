clear all;
close all;
Vt=200/3600; % miles/s
Vg=600/3600; % Ground speed of the UAV
Rhod=5; % Expected ground distance between the target and the UAV
h=.05;% time step
tf=20000; %final time
t=0:h:tf; % time matrix

%k1,k2 dynamic of the feedback
k1=20;
k2=20;

%Xv,Yv position of the vehicle (target)
Xv=zeros(1,tf+1); 
Xv=Xv+5;
Yv=zeros(1,tf+1); 

%Xg,Yg ground projections of the UAV
Xg=zeros(1,tf+1); 
Yg=zeros(1,tf+1);


Lambda=zeros(1,tf+1);
Epsilon=zeros(1,tf+1);%Epsilon is the error between the 1st angle of the gimbal and the LOS
Eta=zeros(1,tf+1); %Eta is the error between the heading angle and the perpendicular to the LOS
Rho=zeros(1,tf+1);% Rho is the ground distance between the Target vehicle and the UAV

%--------------------------------Trajectories------------------------------

%Sinusoidal trajectory
% for k=1:tf
%        Xv(k+1)=Xv(k)+Vt*h;
%    Yv(k+1)=10*sin(Xv(k+1)/5);
% end

%--------------------------------------------------------------------------
%U turn
% 
% for k=1:floor(tf*4/10)
%     Yv(k+1)=Yv(k)+Vt*h;
% end
% for k=floor(tf*4/10)+1:floor(tf*6/10)
%     Xv(k+1)=Xv(k)+Vt*h;
%     Yv(k+1)=Yv(k);
% end
% for k=floor(tf*6/10)+1:tf
%     Yv(k+1)=Yv(k)-Vt*h;
%     Xv(k+1)=Xv(k);
% end

%--------------------------------------------------------------------------
%Acceleration
for k=1:tf
    Yv(k+1)=Yv(k)+Vt*h*2*k/tf;
end

    






Psi=zeros(1,tf+1); %heading angle
Psih=zeros(1,tf+1); %gimbal angle regarding the UAV
dPsi=zeros(1,tf+1);
dPsih=zeros(1,tf+1);
x=zeros(1,tf+1);
y=zeros(1,tf+1);
for k=1:tf
    
    %defining angles
    Lambda(k)=-atan2(Yv(k)-Yg(k),Xv(k)-Xg(k))+pi/2;
    Epsilon(k)=Lambda(k)-Psi(k)-Psih(k);
    Eta(k)=Psi(k)-(Lambda(k)-pi/2);
    
    %Control laws
    dPsi(k)=Vg/Rhod*cos(Eta(k))-k1*Eta(k);
    dPsih(k)=k1*Eta(k)+k2*Epsilon(k);
    
    %Updating angles
    Psi(k+1)=Psi(k)+h*dPsi(k);
    Psih(k+1)=Psih(k)+h*dPsih(k);
    Xg(k+1)=Xg(k)+Vg*sin(Psi(k+1))*h;
    Yg(k+1)=Yg(k)+Vg*cos(Psi(k+1))*h;
    
    Rho(k)=sqrt((Xg(k)-Xv(k))^2+(Yg(k)-Yv(k))^2);
    [x(k),y(k)]=pol2cart(pi/2-(Psi(k)+Psih(k)),1); %drawn gimbal vectors for the final plot
    
end


 figure()
 plot(Xv,Yv,Xg,Yg);
 hold on;
 plot(Xv(1:1000:tf+1),Yv(1:1000:tf+1),'bo');
 hold on;
 quiver(Xg(2:1000:tf+1),Yg(2:1000:tf+1),x(2:1000:tf+1),y(2:1000:tf+1),0.2,'r');
 legend('Target','UAV','','gimbal');
 axis equal;
% figure()
% plot(t(1:tf+1),Lambda);
% legend('Lambda');
% figure()
% plot(t(1:tf+1),Rho);
% legend('Ground distance');
figure()
plot(t(1:tf+1),Epsilon);
legend('Gimbal error Epsilon');
% figure()
% plot(t(1:tf+1),Eta);
% legend('Eta')




    
