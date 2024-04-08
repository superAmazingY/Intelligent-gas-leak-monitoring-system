import service from "@/plugins/axios";
export const loginAPI = (userId, password) => {
    const url = `http://8.130.146.112:8099/user/login?userId=${userId}&passwd=${password}`;
    // 发起网络请求
    return service({
        url: url,
        method: 'POST'
    });
}
export const registerAPI = (userId, password,phoneNumber) => {
    const url =`http://8.130.146.112:8099/user/register?userId=${userId}&passwd=${password}&phone_number=${phoneNumber}`;
    // 发起网络请求
    return service({
        url: url,
        method: 'POST'
    });
}
export const CO2ValueAPI = () => {
    const url =`http://8.130.146.112:8099/user/Co2Info`;
    return service({
        url: url,
        method: 'GET'
    });
}
export const COValueAPI = () => {
    const url =`http://8.130.146.112:8099/user/CoInfo`;
    // 发起网络请求
    return service({
        url: url,
        method: 'GET'
    });
}
export const CH4ValueAPI = () => {
    const url =`http://8.130.146.112:8099/user/CH4Info`;
    return service({
        url: url,
        method: 'GET'
    });
}
export const H2ValueAPI = () => {
    const url =`http://8.130.146.112:8099/user/H2Info`;
    return service({
        url: url,
        method: 'GET'
    });
}

export const ErrorInfoAPI = () => {
    const url =`http://8.130.146.112:8099/user/ERRORInfo`;
    return service({
        url: url,
        method: 'GET'
    });
}

export const fetchDevData = () => {
    const params = {
        author_key: 'BGjW9C6BkVrAyiyIK9gGihC/aiVZpACBsNKeW1dvXifY3jCbAqO5AvydhSum7X+3c86hRjvl/T/7OUpoSN1OOg==',
        version: '2022-05-01',
        user_id: '376681',
    }
    const url = 'https://iot-api.heclouds.com/thingmodel/query-device-property';
    const data = {
        product_id: 'CFcMIQYDRa',
        device_name: 'D1'
    };
    const headers = {
        'authorization': createCommonToken(params)
    };

    return service({
        url: url,
        method: 'GET',
        params: data,
        headers: headers
    }).then(response => {
        console.log(response.data);
        const data = response.data.data;
        const [BEEP, CH4, CO, CO2, H2, led] = data.map(item => item.value);
        return { BEEP, CH4, CO, CO2, H2, led };
    }).catch(error => {
        console.error('Error fetching device data:', error);
        throw error;
    });
};
