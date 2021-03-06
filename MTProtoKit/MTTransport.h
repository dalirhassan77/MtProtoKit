

#import <Foundation/Foundation.h>

@class MTContext;
@class MTDatacenterAddress;
@class MTTransport;
@class MTTransportTransaction;
@class MTOutgoingMessage;
@class MTIncomingMessage;
@class MTMessageTransaction;
@class MTNetworkUsageCalculationInfo;
@class MTSocksProxySettings;

#if defined(MtProtoKitDynamicFramework)
#   import <MTProtoKitDynamic/MTMessageService.h>
#elif defined(MtProtoKitMacFramework)
#   import <MTProtoKitMac/MTMessageService.h>
#else
#   import <MTProtoKit/MTMessageService.h>
#endif

@protocol MTTransportDelegate <NSObject>

@optional

- (void)transportNetworkAvailabilityChanged:(MTTransport *)transport isNetworkAvailable:(bool)isNetworkAvailable;
- (void)transportConnectionStateChanged:(MTTransport *)transport isConnected:(bool)isConnected proxySettings:(MTSocksProxySettings *)proxySettings;
- (void)transportConnectionContextUpdateStateChanged:(MTTransport *)transport isUpdatingConnectionContext:(bool)isUpdatingConnectionContext;
- (void)transportConnectionProblemsStatusChanged:(MTTransport *)transport hasConnectionProblems:(bool)hasConnectionProblems isProbablyHttp:(bool)isProbablyHttp;

- (void)transportReadyForTransaction:(MTTransport *)transport transportSpecificTransaction:(MTMessageTransaction *)transportSpecificTransaction forceConfirmations:(bool)forceConfirmations transactionReady:(void (^)(NSArray *))transactionReady;
- (void)transportHasIncomingData:(MTTransport *)transport data:(NSData *)data transactionId:(id)transactionId requestTransactionAfterProcessing:(bool)requestTransactionAfterProcessing decodeResult:(void (^)(id transactionId, bool success))decodeResult;
- (void)transportTransactionsMayHaveFailed:(MTTransport *)transport transactionIds:(NSArray *)transactionIds;
- (void)transportReceivedQuickAck:(MTTransport *)transport quickAckId:(int32_t)quickAckId;
- (void)transportDecodeProgressToken:(MTTransport *)transport data:(NSData *)data token:(int64_t)token completion:(void (^)(int64_t token, id progressToken))completion;
- (void)transportUpdatedDataReceiveProgress:(MTTransport *)transport progressToken:(id)progressToken packetLength:(NSInteger)packetLength progress:(float)progress;

@end

@interface MTTransport : NSObject <MTMessageService>

@property (nonatomic, weak) id<MTTransportDelegate> delegate;

@property (nonatomic, strong, readonly) MTContext *context;
@property (nonatomic, readonly) NSInteger datacenterId;
@property (nonatomic, strong, readonly) MTDatacenterAddress *address;
@property (nonatomic, strong, readonly) MTSocksProxySettings *proxySettings;
@property (nonatomic) bool simultaneousTransactionsEnabled;
@property (nonatomic) bool reportTransportConnectionContextUpdateStates;

- (instancetype)initWithDelegate:(id<MTTransportDelegate>)delegate context:(MTContext *)context datacenterId:(NSInteger)datacenterId address:(MTDatacenterAddress *)address proxySettings:(MTSocksProxySettings *)proxySettings usageCalculationInfo:(MTNetworkUsageCalculationInfo *)usageCalculationInfo;

- (void)setUsageCalculationInfo:(MTNetworkUsageCalculationInfo *)usageCalculationInfo;

- (bool)needsParityCorrection;

- (void)reset;
- (void)stop;
- (void)updateConnectionState;
- (void)setDelegateNeedsTransaction;
- (void)_processIncomingData:(NSData *)data transactionId:(id)transactionId requestTransactionAfterProcessing:(bool)requestTransactionAfterProcessing decodeResult:(void (^)(id transactionId, bool success))decodeResult;
- (void)_networkAvailabilityChanged:(bool)networkAvailable;

- (void)activeTransactionIds:(void (^)(NSArray *activeTransactionId))completion;

@end
