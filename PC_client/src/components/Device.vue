<template>
  <div>
    <el-form :inline="true" style="margin-bottom: 20px;">
      <el-form-item label="搜索设备">
        <el-input
            v-model="searchInput"
            placeholder="请输入设备ID">
        </el-input>
      </el-form-item>
      <el-form-item>
        <el-button
            type="primary"
            @click="showAddDialog = true">
          添加设备
        </el-button>
      </el-form-item>
    </el-form>

    <el-dialog
        title="添加设备"
        :visible.sync="showAddDialog"
        width="30%">
      <el-form ref="addForm" :model="addForm">
        <el-form-item label="设备ID" prop="deviceId">
          <el-input v-model="addForm.deviceId"></el-input>
        </el-form-item>
      </el-form>
      <span slot="footer" class="dialog-footer">
        <el-button @click="showAddDialog = false">取消</el-button>
        <el-button type="primary" @click="handleAdd">添加</el-button>
      </span>
    </el-dialog>

    <el-table
        :data="filteredTableData"
        style="width: 100%">
      <el-table-column
          prop="deviceId"
          label="设备ID">
      </el-table-column>
      <el-table-column
          prop="location"
          label="设备地点">
      </el-table-column>
      <el-table-column
          prop="onlineStatus"
          label="在线状态">
      </el-table-column>
      <el-table-column
          label="操作">
        <template slot-scope="scope">
          <el-button
              type="text"
              @click="handleDelete(scope.$index)">
            删除设备
          </el-button>
        </template>
      </el-table-column>
    </el-table>
  </div>
</template>

<script>
export default {
  data() {
    return {
      tableData: [
        { deviceId: 'D1', location: '重庆市万州区五桥天星路666号', onlineStatus: '在线' },
        { deviceId: 'D2', location: '重庆市沙坪坝区渝培路', onlineStatus: '离线' },
        { deviceId: 'D8', location: '重庆市九龙坡区奥体三支路', onlineStatus: '在线' },
        // 其他设备数据...
      ],
      searchInput: '',
      showAddDialog: false,
      addForm: {
        deviceId: ''
      }
    };
  },
  computed: {
    filteredTableData() {
      if (!this.searchInput) return this.tableData;
      return this.tableData.filter(item => item.deviceId.includes(this.searchInput.trim()));
    }
  },
  methods: {
    handleDelete(index) {
      this.tableData.splice(index, 1);
    },
    handleAdd() {
      this.tableData.push({
        deviceId: this.addForm.deviceId,
        location: '新地点',
        onlineStatus: '新状态'
      });
      this.showAddDialog = false;
      this.$refs.addForm.resetFields(); // 重置表单
    }
  }
};
</script>

<style>
/* 可以添加一些自定义样式 */
</style>
