<template>
  <view>
    <!-- 添加标题 -->
    <view class="title">智能语音气体监测系统</view>

    <!-- 原有内容 -->
    <view class="wrap">
      <view class="dev-area">
        <view class="dev-cart">
          <view class="">
            <view class="dev-name">CH4</view>
            <image class="dev-logo" src="../../static/CH4.png" mode=""></image>
          </view>
          <view class="dev-data">{{CH4}} PM</view>
        </view>
        <view class="dev-cart">
          <view class="">
            <view class="dev-name">H2</view>
            <image class="dev-logo" src="../../static/H2.png" mode=""></image>
          </view>
          <view class="dev-data">{{H2}} PM</view>
        </view>
        <view class="dev-cart">
          <view class="">
            <view class="dev-name">CO</view>
            <image class="dev-logo" src="../../static/CO.png" mode=""></image>
          </view>
          <view class="dev-data">{{CO}} PM</view>
        </view>
        <view class="dev-cart">
          <view class="">
            <view class="dev-name">CO2</view>
            <image class="dev-logo" src="../../static/CO2.png" mode=""></image>
          </view>
          <view class="dev-data">{{CO2}} PM</view>
        </view>
        <view class="dev-cart">
          <view class="">
            <view class="dev-name">报警灯</view>
            <image class="dev-logo" src="../../static/Led.png" mode=""></image>
          </view>
          <switch :checked="led" @change="onLedSwitch" color="#8f8f94"/>
        </view>
        <view class="dev-cart">
          <view class="">
            <view class="dev-name">报警器</view>
            <image class="dev-logo" src="../../static/Beep.png" mode=""></image>
          </view>
          <switch :checked="BEEP" @change="onBeepSwitch" color="#8f8f94"/>
        </view>
      </view>
    </view>
  </view>
</template>

<script>
  const { createCommonToken } = require('@/key.js')
  export default {
    data() {
      return {
        BEEP: true,
        CH4:'',
        CO:'',
        CO2:'',
        H2:'',
        led:true,
        token: '',
      }
    },
    onLoad() {
      const params = {
        author_key: 'BGjW9C6BkVrAyiyIK9gGihC/aiVZpACBsNKeW1dvXifY3jCbAqO5AvydhSum7X+3c86hRjvl/T/7OUpoSN1OOg==',
        version: '2022-05-01',
        user_id: '376681',
      }
      this.token = createCommonToken(params);
    },
    onShow() {
      this.fetchDevData();
       setInterval(()=>{
		this.fetchDevData();
      },2000)
    },
    methods: {
      fetchDevData(){
        uni.request({
          url: 'https://iot-api.heclouds.com/thingmodel/query-device-property',
          method: 'GET',
          data: {
            product_id: 'CFcMIQYDRa',
            device_name: 'D1'
          },
          header: {
            'authorization': this.token //自定义请求头信息
          },
          success: (res) => {
            console.log(res.data);
            this.BEEP=res.data.data[0].value==='true';
            this.CH4=res.data.data[1].value;
            this.CO=res.data.data[2].value;
            this.CO2=res.data.data[3].value;
            this.H2=res.data.data[4].value;
            this.led=res.data.data[5].value==='true';
          }
        });
      },
      onLedSwitch(event) {
        console.log(event.detail.value);
        let value = event.detail.value;
        uni.request({
          url: 'https://iot-api.heclouds.com/thingmodel/set-device-property',
          method: 'POST',
          data: {
            product_id: 'CFcMIQYDRa',
            device_name: 'D1',
            params: { "led": value }
          },
          header: {
            'authorization': this.token
          },
          success: () => {
            console.log('LED ' + (value ? 'ON' : 'OFF') + ' !');
          }
        });
      },
      onBeepSwitch(event) {
        console.log(event.detail.value);
        let value = event.detail.value;
        uni.request({
          url: 'https://iot-api.heclouds.com/thingmodel/set-device-property',
          method: 'POST',
          data: {
            product_id: 'CFcMIQYDRa',
            device_name: 'D1',
            params: { "BEEP": value }
          },
          header: {
            'authorization': this.token
          },
          success: () => {
            console.log('BEEP ' + (value ? 'ON' : 'OFF') + ' !');
          }
        });
      }
    }
  }
</script>

<style scoped>
  .title {
    font-size: 48rpx; /* 修改标题字体大小为48rpx */
    text-align: center;
    margin-bottom: 20rpx;
  }
  .wrap {
    padding: 40rpx;
  }
  .dev-area {
    display: flex;
    justify-content: space-between;
    flex-wrap: wrap;
  }
  .dev-cart {
    height: 150rpx;
    width: 320rpx;
    border-radius: 30rpx;
    margin-top: 30rpx;
    display: flex;
    justify-content: space-around;
    align-items: center;
    box-shadow: 0 0 15rpx #ccc;
  }
  .dev-name {
    font-size: 25rpx;
    text-align: center;
    color: #6d6d6d;
  }
  .dev-logo {
    width: 80rpx;
    height: 80rpx;
    margin-top: 0rpx;
  }
  .dev-data {
    font-size: 40rpx;
    color: #6d6d6d;
  }
</style>