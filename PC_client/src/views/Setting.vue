<template>
  <div class="card-container">
    <el-card class="box-card">
      <p>一氧化碳传感器</p>
      <el-slider
          v-model="CO_value"
          vertical
          height="350px"
          :min=0
          :max=50>
      </el-slider>
      <p>当前一氧化碳上限:{{CO_value}}</p>
      <el-switch
          v-model="switch_CO"
          active-text="on"
          inactive-text="off">
      </el-switch>
    </el-card>

    <el-card class="box-card">
      <p>二氧化碳传感器</p>
      <el-slider
          v-model="CO2_value"
          vertical
          height="350px"
          :min=300
          :max=2000>
      </el-slider>
      <p>当前二氧化碳上限:{{CO2_value}}</p>
      <el-switch
          v-model="switch_CO2"
          active-text="on"
          inactive-text="off">
      </el-switch>
    </el-card>

    <el-card class="box-card">
      <p>甲烷传感器</p>
      <el-slider
          v-model="CH4_value"
          vertical
          height="350px"
          :min=0
          :max=50>
      </el-slider>
      <p>当前甲烷碳上限:{{CH4_value}}</p>
      <el-switch
          v-model="switch_CH4"
          active-text="on"
          inactive-text="off">
      </el-switch>
    </el-card>

    <el-card class="box-card">
      <p>氢气传感器</p>
      <el-slider
          v-model="H2_value"
          vertical
          height="350px"
          :min=0
          :max=50>
      </el-slider>
      <p>当前氢气上限:{{H2_value}}</p>
      <el-switch
          v-model="switch_H2"
          active-text="on"
          inactive-text="off">
      </el-switch>
    </el-card>
  </div>
</template>

<script>
export default {
  name: 'SymmetricCards',
  data(){
    return{
      CO_value:10,
      CO2_value:500,
      CH4_value:12,
      H2_value:18,
      switch_CO:false,
      switch_CO2:false,
      switch_CH4:false,
      switch_H2:false,
    }
  },
}
</script>

<style scoped>
.card-container{
  display: flex;
  flex-wrap: wrap;
  justify-content: space-between; /* 控制左右对齐 */
}

.box-card {
  width: 300px;
  height: 620px;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
}

.el-slider.is-vertical/deep/ {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
}

.el-switch/deep/ {
  display: inline-flex;
  justify-content: space-between;
  align-items: center;
  position: relative;
  font-size: 14px;
  line-height: 20px;
  height: 20px;
  vertical-align: middle;
  padding-left: 40px;
}

p{
  text-align: center;
  font-size: 20px;
  margin-top: 40px;
  margin-bottom: 40px;
}
</style>
