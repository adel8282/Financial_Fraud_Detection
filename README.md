# Food Delivery & Management System  

A comprehensive platform for managing food delivery, takeaway, and dine-in services. This system involves **Vendors**, **Customers**, **Delivery Couriers**, **Admins**, and **Managers** with unique functionalities and a defined **order status flow**.

**Form Size for Menus Should Be 900x660**

---

## 📋 Features  

### **Vendor - Osamah** 
  - Create and manage menu items.   
  - Accept/cancel orders.
  - Generate delivery task after accepting a delivery order.
  - Update order statuses.  
  - View order history with filters (1M/6M/12M).  
  - View revenue with filters (1M/6M/12M).  
  - Read customer reviews.  

---

### **Customer - Mohammed** 
  - View menus of registered vendors.
  - Add address.
  - Place/cancel orders.   
  - Check order statuses.  
  - View order history with filters (1M/6M/12M).  
  - Submit order reviews.  
  - Send complaints.  
  - View transaction history.

---

### **Delivery Courier - Mahmood**  
  - View and accept/decline delivery tasks.  
  - Update task statuses.  
  - View task history.  
  - Read customer reviews.  
  - View earnings with filters (1M/6M/12M).  

---

### **Admin - Adel**  
  - Register vendors, customers, and couriers.  
  - Top-up customer balances with receipt generation and notification.  

---

### **Manager - Sohaib**  
  - View revenue reports for all vendors.  
  - Review customer complaints.  
  - Manage vendor menus by removing items.  

---

## 🛠️ Order Status Flow  

### **1. Successful Delivery**  
1. **Customer Orders:** `PENDING_VENDOR`  
2. **Vendor Accepts:** `PENDING_COURIER`  
3. **Courier Accepts:** `PREPARING`  
4. **Vendor Hands Over:** `OUT_FOR_DELIVERY`  
5. **Courier Delivers:** `DELIVERED`  

### **2. Unsuccessful Delivery**  
- **Case 1:** Vendor declines → `VENDOR_DECLINED`  
- **Case 2:** No Couriers → `COURIER_DECLINED`  

### **3. Successful Takeaway**  
1. **Customer Orders:** `PENDING_VENDOR`  
2. **Vendor Accepts:** `PREPARING`  
3. **Food Ready:** `READY_FOR_PICKUP`
4. **Order Picked Up:** `PICKED_UP`

### **4. Successful Dine-in**  
1. **Customer Orders:** `PENDING_VENDOR`  
2. **Vendor Accepts:** `PREPARING`  
3. **Food Served:** `ORDER_SERVED`  

### **5. Unsuccessful Takeaway/Dine-in**  
- Vendor declines → `VENDOR_DECLINED`  

---
