# 蓝牙
参考资料 Blueteeth Essentials for Programmers

## 蓝牙简介

* Bluetooth is a way for devices to wirelessly communicate over short distances. Wireless communication has been around since the late nineteenth century, and has taken form in radio, infrared, television, and more recently 802.11. What distinguishes Bluetooth is its special attention to short-distance com- munication, usually less than 30 ft. Both hardware and software are affected by this special attention. 

* 蓝牙是设备短距离无线通信的一种方式。自19世纪后期以来，无线通信已经出现，并且已经在无线电、红外线、电视以及最近的802.11中形成。 蓝牙的区别在于它对短距离通信的特别关注，通常不超过10米。

## 802.11

## 从编程者的角度理解蓝牙
* 重点问题：我们如何创建连接两个蓝牙设备并在它们之间传输数据的程序

## 蓝牙建立连接 OUTGOING/INGOING
### 与互联网TCP/IP协议进行对比
* 与网络编程对比
    1. 蓝牙向外建立连接
        * 搜索附近的设备
        * 询问设备的display name
        * 用user-specified的名字选择一个设备
        * 硬编码的协议 RFCOMM, L2CAP, or SCO
        * 搜索目标设备以查找与预定义标识符（例如UUID，name等）匹配的SDP记录
        * 选择端口号
        * socket() connect()
        * send() recv()
        * close()
    2. 蓝牙处理外部的连接
        * 硬编码的协议
        * 选择端口号
        * 绑定并监听
        * 用本地SDP服务器广告服务
        * 监听接收
        * 发送接收数据
        * 关闭连接

### 选择一个目标设备
* 制造的每个蓝牙芯片都印有 **全球唯一的48位地址，称为蓝牙地址或设备地址。** 这在性质上与以太网的机器地址代码（MAC）地址相同。**实际上，两个地址空间都由同一组织管理 -  IEEE注册机构。** 这些蓝牙地址在制造时分配，旨在保持独特性，并在芯片的生命周期内保持静止。它可以方便地作为所有蓝牙编程中的基本寻址单元。要使一个蓝牙设备与另一个蓝牙设备通信，它必须有某种方法来确定其他设备的蓝牙地址。该地址 **用于蓝牙通信过程的所有层，从低级无线电协议到更高级别的应用协议。** 相反，使用以太网的TCP/IP网络设备在通信过程的较高层丢弃48位MAC地址并切换到使用IP地址。然而，原理保持不变，因为必须知道目标设备的唯一识别地址才能与之通信。客户端可能对这种目标地址一无所知。例如在在因特网编程中，用户通常知道或提供主机名，然后必须通过DNS将其转换为物理IP地址。在蓝牙中，用户通常会提供一些 **用户友好的名称** ，例如“我的电话”，并且客户端通过搜索附近的蓝牙设备并检查每个设备的名称将其转换为数字地址。
    **用户友好的名称** 与DNS相反，设备会先搜索附近的设备地址，再向设备询问用户友好的设备名称。

### 设备发现 地址与类别
* 设备发现，也称为设备查询，是搜索和检测附近蓝牙设备的过程。理论上很简单：要弄清楚附近有什么， **广播“发现”消息并等待回复。每个回复包括响应设备的地址和标识设备的一般类别的整数（例如，手机，台式PC，耳机等）**。然后，可以通过单独联系每个设备来获取更详细的信息，例如设备名称。

### 可发现性和可连接性
出于隐私和电源问题，所有蓝牙设备都有两个选项，用于确定设备是否响应设备查询和连接尝试。 “查询扫描”选项控制前者，“页面扫描”选项控制后者。
* Inquiry Scan  设备是否可被发现
* Page Scan     是否可连接

### 选择一个传输协议
* RFCOMM - TCP
* L2CAP - UDP 尽力而为的服务
* 蓝牙规范定义了四种主要的传输协议。 其中，RFCOMM往往是最佳选择，有时也是唯一的选择。 L2CAP也是一种广泛使用的传输协议，在不需要RFCOMM的流媒体特性时使用。

### 端口号
* 蓝牙协议只有30个端口
* 保留端口 例如，服务发现协议（SDP）使用端口1，RFCOMM连接在L2CAP端口3上复用。RFCOMM没有任何保留端口。

### 服务发现协议
**客户端应用程序** 在建立传出连接时必须回答的一个重要问题是，**服务器正在侦听哪个端口？** 
* 蓝牙试图通过引入SDP来避免这个问题。基本前提是每个蓝牙设备都维护一个SDP服务器监听一个众所周知的端口号。当服务器应用程序启动时，它会在本地设备上向SDP服务器注册自身和端口号的描述。然后，当远程客户端应用程序首次连接到设备时，它会提供它正在搜索到SDP服务器的服务的描述，并且SDP服务器提供所有匹配的服务的列表。
* 通过使用这种方法，蓝牙允许服务器应用程序使用动态分配 - 当服务器启动时，它可以选择未在本地设备上使用的任意端口号，从而确保它没有端口冲突。

1. 服务记录
* 服务器应用向其SDP服务器注册并且SDP服务器向查询客户端发送的服务的描述被称为服务记录，也称为SDP记录。
* 从理论上讲，服务记录非常简单，它由属性/值对列表组成。 
* 属性: 16位整数，每个值可以是基本数据类型，如整数或字符串，或其他数据类型的列表。在某种意义上，服务记录本质上是描述所提供服务的各种条目的字典。
* 在为服务列出的实际端口号旁边，服务记录中可能有两个最重要的属性是服务ID和服务类ID列表。 这两个属性通常是客户端用来实际识别所需服务的属性。

2. 服务ID
* 虽然对服务的描述很好，但仍然需要回答的一个大问题是，**客户如何知道他们寻求的服务描述是什么？**蓝牙采用的方法是为每个服务分配一个唯一的标识符，并确保客户端已经知道它正在寻找的服务的唯一ID。换句话说，开发人员不是在设计时分配端口号，而是在设计时分配唯一的ID。使用唯一ID的最大优势是使用端口号，有效唯一ID的空间设计为远大于有效端口号的空间，从而允许开发人员最大限度地减少唯一ID冲突的可能性。
此方法之前已经完成，并且Internet工程任务组有一个标准方法，供开发人员独立提出自己的128位通用唯一标识符（UUID）。这是SDP围绕其旋转的基本思想，该标识符称为服务的服务ID。具体来说，开发人员在设计时选择此UUID，当程序运行时，它会将包含其服务ID的服务记录与该设备的SDP服务器一起注册。尝试查找特定服务的客户端应用程序将查询它找到的每个设备上的SDP服务器，以查看该设备是否提供具有相同UUID的任何服务。
UUID的标准符号是以“XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX”形式的连字符分隔的数字系列，其中每个“X”是十六进制数字。八位的第一段对应于UUID的位1-32，四位的下一段是位33-36，依此类推。

3. 服务类ID列表
* 虽然服务ID本身可以在识别服务和找到我们想要的服务方面花费很长的时间，但它真正意味着由单个开发团队构建的自定义应用程序。蓝牙设计人员希望区分这些自定义应用程序和所有执行相同操作的应用程序类别。例如，两家不同的公司可能都会发布通过蓝牙提供音频服务的蓝牙软件。尽管它们是由不同的人编写的完全不同的程序，但它们都做同样的事情。为了解决这个问题，蓝牙**引入了第二个UUID，称为服务类ID。**现在，两个不同的程序可以只宣传相同的服务类ID，并且所有程序都可以在蓝牙土地上使用。当然，只有当两家公司就要使用的服务类ID达成一致时，这才有用。
另一个需要考虑的问题是：**如果我有一个可以提供多种服务的应用程序怎么办？**例如，许多蓝牙耳机可以用作简单的耳机和扬声器，并宣传该服务类;但他们也能够控制电话 - 拨打来电，静音麦克风，挂机等等。虽然在这种情况下可以只注册两个单独的服务，每个服务都有一个特定的服务类，但蓝牙设计者选择允许每个服务都有一个服务提供的服务类列表。因此，**虽然单个服务只能有一个服务ID，但它可以有许多服务类ID。** 

4. 蓝牙保留UUIDs
与L2CAP和TCP为特殊目的保留端口号的方式类似，蓝牙也保留了UUID。 偶尔称为短UUID，这些UUID主要用于识别预定义的服务类，但也用于传输协议和配置文件。 所有保留的蓝牙UUID的低96位是相同的，因此它们通常由它们的高16位或32位来表示。 表1.3中显示了一些较常见的保留蓝牙UUID。
要从16位或32位数字获取完整的128位UUID，请使用蓝牙基础UUID（00000000-0000-1000-8000-00805F9B34FB）并将最左侧的段替换为16位或32位值。 在数学上，这是相同的
128 bit UUID = 16 or 32 bit number ∗ 296 + Bluetooth Base UUID

5. 常见SDP属性
    * Service Class ID List
        服务提供的服务类UUID列表。 这是必须始终出现在服务记录中的唯一必需属性。
    * Service ID
        标识特定服务的单个UUID。
    * Service Name
        描述所提供服务的文本字符串。
    * Service Description
        服务使用的协议和端口号列表。
    * Service Description List
        服务符合的蓝牙配置文件描述符列表。每个描述符由UUID和版本号组成。
    * Service Record Handle
        单个无符号整数，用于唯一标识设备中的记录。

### 通过套接字进行消息传递
1. 客户端套接字
* 客户端套接字相当简单，因为一旦创建了套接字，只需要采取一个额外的步骤来获得连接的套接字。 这是发出connect命令，指定目标设备的地址和端口。 然后，操作系统负责所有较低级别的详细信息，保留本地蓝牙适配器上的资源，搜索远程设备，形成微微网以及建立连接。 连接套接字后，它可用于数据传输。

2. 服务器/监听套接字
* 在获取连接套接字时，服务器套接字更复杂一些。必须采取三个步骤：首先，应用程序必须将套接字绑定到本地蓝牙资源，指定本地蓝牙适配器和要使用的端口号。*其次，listen命令用于将套接字置于侦听模式。这指示操作系统接受本地适配器上的传入连接请求和套接字绑定的端口号。最后，accept命令用于获取可用于数据传输的连接套接字。
* 服务器套接字和客户端套接字之间的**主要区别之一**是，**应用程序首先创建的服务器套接字永远不能用于实际通信。相反，每次服务器套接字使用accept命令接受新的传入连接时，它会生成一个代表新建立的连接的全新套接字。**然后，服务器套接字返回监听更多连接请求，应用程序应使用新创建的套接字与客户端进行通信。

3. 利用一个连接套接字进行消息传递
一旦蓝牙应用程序具有连接的套接字，使用它进行通信就很简单了。发送和接收命令用于好吧，发送和接收数据。发送推送字节，但其成功返回**仅表示字节已从应用程序地址空间移动到操作系统的缓冲区**。他们可能已经或可能没有离开设备。接收至少返回一些数据。 它会阻塞，直到数据，甚至1个字节到达。 仅当连接断开时，接收才会返回0字节。用程序完成后，它只是调用close命令断开套接字。 关闭侦听服务器套接字将取消绑定端口并停止接受传入连接。

4. Select非阻塞套接字
* 在到目前为止描述的通信例程中，通常**存在某种等待**。在此期间，控制线程阻塞（等待）并且不能执行任何其他操作，例如响应用户输入或在另一个套接字上操作。对于简单的应用程序，这可能是好的（甚至可能是合乎需要的），但对于需要同时执行其他任务的更复杂的程序是不可接受的。为了避免同步编程的这个缺陷，可以使用多个控制线程，其中一个线程专用于需要一些阻塞的每个任务。但是，这可能会变得非常繁琐和复杂，因此我们将转向使用**异步技术**作为解决方案。
* 异步套接字编程中的主要工具是单个事件循环。我们的想法是，**应用程序不会等待单个事件发生（传入连接或新接收的数据），而是等待所有事情**。如果发生单个感兴趣事件，则应用程序处理该事件并返回到事件循环，等待更多事件发生。
为了使事件循环正常工作，必须快速处理事件，不得有任何延迟。因此，**函数调用不应该阻塞，必须立即返回**。为了支持这一点，可以将套接字切换到非阻塞模式，这样通常阻止的所有操作都会立即返回。首次创建时，蓝牙套接字默认为阻塞套接字。表1.4描述了导致阻止套接字返回操作的事件。如果未列出，则操作通常立即返回。
* 由于在非阻塞套接字上执行这些操作不再通知应用程序何时发生这些事件，因此我们需要一种不同的方式来等待它们。后续章节中讨论的主要编程环境都支持select操作，后者提供此功能。我们将以不同的形式看到它，但是对select的调用通常需要四个参数：

    **select( read_set, write_set, exception_set, timeout)**

* 前三个参数中的每一个都**包含一组套接字**。select操作等待其中一个事件发生或超时期限到期。请注意，第三组异常集与任何类型的套接字的任何事件都不对应。虽然它在其他类型的套接字中有意义，例如TCP套接字，但在蓝牙套接字的主要选择实现中不使用此套件。
* 如果select只能用于检测蓝牙套接字的事件，那么它仍然没有那么有用。但是，大多数实现可以融合许多不同I/O模式的事件检测，包括TCP套接字，文件句柄和用户输入句柄。从这个角度来看，主要基于I/O操作的应用程序可以通过基于选择的事件循环得到很好的服务。
