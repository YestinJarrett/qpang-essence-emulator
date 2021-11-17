#include "LobbyPacketHandler.h"

#include "HandleEnchantItemRequest.h"
#include "HandleGiftCardRequest.h"
#include "HandleHandshakeRequest.h"
#include "HandleLoginRequest.h"
#include "HandleTradeRequest.h"
#include "account/HandleAccountRegistrationRequest.h"
#include "account/HandleReferralRegistrationRequest.h"
#include "channel/HandleRequestChannelHost.h"
#include "channel/HandleRequestChannelList.h"
#include "crane/HandleUseCraneRequest.h"
#include "dailybonus/HandleGetDailyBonusRequest.h"
#include "equipment/HandleEquipArmorRequest.h"
#include "equipment/HandleEquipWeaponRequest.h"
#include "equipment/HandleGetEquippedSkillCardsRequest.h"
#include "equipment/HandleUpdateSkillSetRequest.h"
#include "friend/HandleAcceptIncomingFriendRequest.h"
#include "friend/HandleCancelOutgoingFriendRequest.h"
#include "friend/HandleDenyIncomingFriendRequest.h"
#include "friend/HandleRemoveFriendRequest.h"
#include "friend/HandleRequestFriendList.h"
#include "friend/HandleSendFriendRequest.h"
#include "gameroom/HandleGameRoomInviteRequest.h"
#include "gameroom/HandleGetGameRoomsRequest.h"
#include "gameroom/HandleGetGameSettingsRequest.h"
#include "inventory/HandleDeleteInventoryCardRequest.h"
#include "inventory/HandleDisableFunctionCardRequest.h"
#include "inventory/HandleEnableFunctionCardRequest.h"
#include "inventory/HandleRenewInventoryCardRequest.h"
#include "inventory/HandleGetGiftsRequest.h"
#include "inventory/HandleGetInventoryRequest.h"
#include "inventory/HandleOpenGiftRequest.h"
#include "inventory/HandleOpenInventoryCardRequest.h"
#include "memo/HandleGetMemosRequest.h"
#include "nickname/HandleChangeNickNameRequest.h"
#include "nickname/HandleValidateNickNameRequest.h"
#include "player/HandleChangePlayerCharacterRequest.h"
#include "player/HandleGetPlayerCashBalanceRequest.h"
#include "player/HandleGetPlayerInfoRequest.h"
#include "player/HandleGetPlayerRankingRequest.h"
#include "player/HandlePlayerWhisperRequest.h"
#include "player/HandleResetPlayerKillDeathRequest.h"
#include "player/HandleResetPlayerWinLossRequest.h"
#include "redeemcode/HandleUseRedeemCodeRequest.h"
#include "shop/HandleBuyShopCardRequest.h"
#include "shop/HandleGetShopItemsRequest.h"
#include "shop/HandleGetShopPackagesRequest.h"
#include "shop/HandleSendGiftInShopRequest.h"
#include "training/HandleTrainingRequest.h"

LobbyPacketHandler::LobbyPacketHandler() : PacketHandler()
{
	add(1, new HandleHandshakeRequest());
	add(600, new HandleLoginRequest());
	add(620, new HandleEquipArmorRequest());
	add(623, new HandleEquipWeaponRequest());
	add(646, new HandleGetEquippedSkillCardsRequest());
	add(649, new HandleUpdateSkillSetRequest());
	add(652, new HandleDeleteInventoryCardRequest());
	add(655, new HandleOpenInventoryCardRequest());
	add(667, new HandleValidateNickNameRequest());
	// 670 - Choose nickname and characters, create account.
	add(670, new HandleAccountRegistrationRequest());
	// 767 - Validate/check friend referral.
	add(676, new HandleReferralRegistrationRequest());
	add(679, new HandleChangePlayerCharacterRequest());
	add(691, new HandleGetPlayerInfoRequest());
	add(694, new HandleRequestFriendList());
	add(697, new HandleSendFriendRequest());
	add(701, new HandleAcceptIncomingFriendRequest());
	add(705, new HandleDenyIncomingFriendRequest());
	add(709, new HandleCancelOutgoingFriendRequest());
	add(713, new HandleRemoveFriendRequest());
	// 717 - Invite friend to join the gameroom
	add(721, new HandleGameRoomInviteRequest());
	add(725, new HandleGetMemosRequest());
	// 728 - Send memo
	add(738, new HandlePlayerWhisperRequest());
	add(742, new HandleOpenGiftRequest());
	add(745, new HandleGetGiftsRequest());
	// 751 - Has something to do with joining a room (also occurs with creating a room)
	add(758, new HandleGetGameRoomsRequest());
	add(762, new HandleRequestChannelList());
	add(766, new HandleRequestChannelHost());
	add(769, new HandleGetGameSettingsRequest());
	add(780, new HandleGetInventoryRequest());
	add(791, new HandleGetPlayerRankingRequest());
	add(797, new HandleGetShopItemsRequest());
	add(800, new HandleGetShopPackagesRequest());
	add(803, new HandleBuyShopCardRequest());
	add(809, new HandleRenewInventoryCardRequest());
	add(812, new HandleGiftCardRequest());
	add(815, new HandleSendGiftInShopRequest());
	// 826 = Trigger NTS_ROY_NEWBIE_EVENT_DESC_START_1 (outgoing)
	add(828, new HandleTrainingRequest());
	add(831, new HandleGetPlayerCashBalanceRequest());
	add(834, new HandleEnableFunctionCardRequest());
	add(837, new HandleChangeNickNameRequest());
	add(841, new HandleResetPlayerWinLossRequest());
	add(844, new HandleResetPlayerKillDeathRequest());
	add(851, new HandleUseRedeemCodeRequest());
	add(861, new HandleDisableFunctionCardRequest());
	// 875 - Send trade request
	add(875, new HandleTradeRequest());
	// 884 - Trade cancel?
	add(897, new HandleUseCraneRequest());
	add(901, new HandleGetDailyBonusRequest());
	add(903, new HandleEnchantItemRequest());
	// 903 - Request to boost clothing piece
	// 906 - Panthalassa box opening
}
