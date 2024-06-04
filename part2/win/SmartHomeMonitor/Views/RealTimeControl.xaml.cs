using LiveChartsCore.SkiaSharpView.Drawing;
using LiveChartsCore;
using LiveChartsCore.SkiaSharpView.Extensions;
using LiveChartsCore.VisualElements;
using MahApps.Metro.Controls;
using MQTTnet;
using MQTTnet.Client;
using Newtonsoft.Json;
using SmartHomeMonitoringApp.Logics;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;
using LiveChartsCore.SkiaSharpView.VisualElements;
using LiveChartsCore.Defaults;
using LiveChartsCore.SkiaSharpView;

namespace SmartHomeMonitoringApp.Views
{
    /// <summary>
    /// RealTimeControl.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class RealTimeControl : UserControl
    {
        public RealTimeControl()
        {
            InitializeComponent();

            DispatcherTimer timer = new DispatcherTimer();
            //DispatcherTimer: 주기적으로 이벤트를 발생시키는 타이머입니다.
            //여기서는 1초 간격(new TimeSpan(0,0,1))으로 설정되었습니다.
            timer.Interval = new TimeSpan(0,0,1); // 1초마다
            timer.Tick += Timer_Tick;
            timer.Start();
        }

        private void Timer_Tick(object sender, EventArgs e)
        {
            LblSensingDt.Content = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");
        }

        private async void UserControl_Loaded(object sender, RoutedEventArgs e)
        {
            // 만약 Commons.MQTT_CLIENT가 이미 존재하고 연결되어 있다면
            if (Commons.MQTT_CLIENT != null && Commons.MQTT_CLIENT.IsConnected)
            {
                // 다른 화면에서 이미 MQTT를 연결했다면
                // 메시지를 받을 이벤트 핸들러를 추가
                Commons.MQTT_CLIENT.ApplicationMessageReceivedAsync += MQTT_CLIENT_ApplicationMessageReceivedAsync;
            }
            else
            {
                // MQTT 클라이언트가 존재하지 않거나 연결되지 않았다면
                var mqttFactory = new MqttFactory();
                Commons.MQTT_CLIENT = mqttFactory.CreateMqttClient();

                // MQTT 클라이언트 옵션을 설정 (TCP 서버 호스트명 사용)
                var mqttClientOptions = new MqttClientOptionsBuilder()
                                            .WithTcpServer(Commons.BROKERHOST)
                                            .Build();

                // MQTT 클라이언트를 연결
                await Commons.MQTT_CLIENT.ConnectAsync(mqttClientOptions, CancellationToken.None);

                // 메시지를 받을 이벤트 핸들러를 추가
                Commons.MQTT_CLIENT.ApplicationMessageReceivedAsync += MQTT_CLIENT_ApplicationMessageReceivedAsync;

                // MQTT 구독 옵션 설정 (주제 설정)
                var mqttSubscribeOptions = mqttFactory.CreateSubscribeOptionsBuilder()
                                                      .WithTopicFilter(f =>
                                                      {
                                                          f.WithTopic(Commons.MQTTTOPIC);
                                                      })
                                                      .Build();

                // 주제에 구독 요청
                await Commons.MQTT_CLIENT.SubscribeAsync(mqttSubscribeOptions, CancellationToken.None);
            }
        }

        private Task MQTT_CLIENT_ApplicationMessageReceivedAsync(MQTTnet.Client.MqttApplicationMessageReceivedEventArgs arg)
        {
            var payload = Encoding.UTF8.GetString(arg.ApplicationMessage.Payload);
            Debug.WriteLine(payload);
            UpdateChart(payload);

            return Task.CompletedTask;
        }

        // 앵귤러차트를 위한 변수, 속성 선언
        //int sectionOuter = 130;
        //int sectionWidth = 20;

        public IEnumerable<ISeries> HumidSeries { get; set; }

        public IEnumerable<VisualElement<SkiaSharpDrawingContext>> VisualElements { get; set; }

        public NeedleVisual Needle { get; set; }

        private static void SetStyle(
        double sectionsOuter, double sectionsWidth, PieSeries<ObservableValue> series)
        {
            series.OuterRadiusOffset = sectionsOuter;
            series.MaxRadialColumnWidth = sectionsWidth;
        }

        private void UpdateChart(string payload)
        {
            // 차트에 값 대입해서 차트가 나오도록
            this.Invoke(() =>
            {
                var currValue = JsonConvert.DeserializeObject<Dictionary<string, string>>(payload);

                var splitValue = currValue["VALUE"].Split('|');
                var temp = Convert.ToDouble(splitValue[0]);
                var humid = Convert.ToDouble(splitValue[1]);

                var tempVal = GaugeGenerator.BuildSolidGauge(new GaugeItem(
                        temp,
                        series =>
                        {
                            series.MaxRadialColumnWidth = 50;
                            series.DataLabelsSize = 50;
                        }
                    ));
                ChtLivingTemp.Series = ChtBathTemp.Series = ChtBedTemp.Series = ChtDiningTemp.Series = tempVal;

                var sectionOuter = 130;
                var sectionWidth = 20;

            // 습도차트 값 할당
                HumidSeries = GaugeGenerator.BuildAngularGaugeSections(
                    new GaugeItem(humid,
                    s => SetStyle(sectionOuter, sectionWidth, s))
                    );
                ChtLivingHumid.Series = ChtBedHumid.Series = ChtBathHumid.Series = ChtDiningHumid.Series = HumidSeries;

                // 습도를 나타낼 앵귤러차트를 초기화
                Needle = new NeedleVisual { Value = humid };
                VisualElements = new VisualElement<SkiaSharpDrawingContext>[]
                {
                new AngularTicksVisual
                {
                    LabelsSize = 16,
                    LabelsOuterOffset = 15,
                    OuterOffset = 65,
                    TicksLength = 15
                },
                Needle
                };

                ChtBathHumid.VisualElements = ChtBedHumid.VisualElements = ChtLivingHumid.VisualElements = ChtDiningHumid.VisualElements = VisualElements; // 위에서 만든 화면디자인을 차트에 적용
            });
        }

        private void BtnWarning_Click(object sender, RoutedEventArgs e)
        {
            Commons.MQTT_CLIENT.PublishStringAsync("pknu/rcv/", "{'control' : 'warning'}");
        }

        private void BtnNormaling_Click(object sender, RoutedEventArgs e)
        {
            Commons.MQTT_CLIENT.PublishStringAsync("pknu/rcv/", "{'control' : 'normal'}");
        }

        private void BtnOffing_Click(object sender, RoutedEventArgs e)
        {
            Commons.MQTT_CLIENT.PublishStringAsync("pknu/rcv/", "{'control' : 'off'}");
        }
    }
}
