%%
V = 12;

Ts = 1e-5;
T.end = 0.3*3;

Rph_to_ph = 0.2;
R = Rph_to_ph/2;
Lph_to_ph = 0.045;
L = Lph_to_ph/2;
ke = 1/1.1;
p = 4;
%%
Kp = 1.59;
Ki = 0.1;
PI_limit = 15;


b = (1-exp(-R*Ts/L))/R;
B = [b 0; 0 b];
J = [0 -1; 1 0];
F = [-R/L 0; 0 -R/L];
A = exp(F*Ts);
a = exp(-R/L*Ts);

g = 0.9;
eta = 7.8821e-04;
m = eta*g/b-0.1;

hyst = 0.1;
eps = 0.00001;

w_max = 10000;
cutoff_freq = 1200;
MaxStatorVoltage = 12;
%%

res = sim("dtc_smo2.slx");
t = res.w.time;
w = [res.w.signals(1).values res.w.signals(2).values res.w.signals(3).values];
err_e = [res.err_e.signals(1).values res.err_e.signals(2).values];
err_T_e = [res.err_T_e.signals.values];
h = figure();
set(h, 'DefaultAxesFontSize', 32, 'DefaultAxesFontName', 'Times New Roman');
tiledlayout(2,2)
nt1 = nexttile(1, [2 1]);
pl = plot(res.w.time, w, LineWidth=3);
pl(1).LineStyle = ":";
pl(2).LineStyle = "-";
pl(3).LineStyle = "--";
ylim([-100 650])
grid on
xlabel('t, c');
ylabel('\omega, рад/с');
legend('$\omega_{ref}$', '$\hat{\omega}_m$', '$\omega_m$', 'Interpreter', 'latex')
xlim([0 0.9])
%h = figure();
%set(h, 'DefaultAxesFontSize', 32, 'DefaultAxesFontName', 'Times New Roman');
%tiledlayout(2,1)
nt2 = nexttile;
plot(res.err_e.time, err_e, LineWidth=1);
grid on
%xlabel('t, c');
xlim([0 0.9])
ylabel('$\tilde{e}_{\alpha\beta}$, B', 'Interpreter', 'latex');
legend('$\tilde{e}_{\alpha}$', '$\tilde{e}_{\beta}$', 'Interpreter', 'latex')
nt3 = nexttile;
plot(res.err_T_e.time, err_T_e, LineWidth=2);
grid on
xlabel('t, c');
ylabel('$\tilde{T}_e$, Hm', 'Interpreter', 'latex');
ylim([-0.01 0.01])
xlim([0 0.9])

linkaxes([nt1 nt2 nt3], 'x')
